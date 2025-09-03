#include "collector.h"

#include <stdio.h>
#include <string.h>

#include "storage.h"

#include <sys/random.h>



enum result_code add_key(struct context *ctx, uint64_t key) {
	struct key_list* key_ = malloc(sizeof(struct key_list));
	if (key_ == nullptr) {
		return FAILURE;
	}

	key_->key = key;
	key_->next = ctx->keys;
	ctx->keys = key_;

	return SUCCESS;
}

struct allocation_result gc_alloc(struct context *ctx, uint32_t size_) {
	uint64_t size = size_;
	uint64_t key;
	struct allocation_result result = {0};

	if (ctx == nullptr) {
		result.code = FAILURE;
		return result;
	}

	ssize_t err = getrandom(&key, 8, GRND_NONBLOCK);
	if (err < 0) {
		result.code = FAILURE;
		return result;
	}

	struct region *region = malloc(sizeof(struct region));
	if (region == nullptr) {
		result.code = FAILURE;
		return result;
	}

	region->size = size;
	region->reference_count = 1;
	region->region = malloc(region->size);

	if (region->region == nullptr) {
		result.code = FAILURE;
		free(region);
		return result;
	}

	enum result_code tmp = add_key(ctx, key);
	if (tmp != SUCCESS) {
		result.code = FAILURE;
		free(region->region);
		free(region);
		return result;
	}

	struct item item;
	item.key = key;
	item.ptr = region;

	result.code = store_item(item);
	if (result.code != SUCCESS) {
		free(region->region);
		free(region);
		return result;
	}

	result.key = key;
	return result;
}

enum result_code clear_scope(struct context *ctx) {
	if (ctx == nullptr) return FAILURE;
	struct key_list *curr = ctx->keys;
	ctx->keys = nullptr;

	while (curr != nullptr) {
		uint64_t key = curr->key;
		struct result res = get_item(key);
		if (res.code != SUCCESS) {
			return FAILURE;
		}

		struct region *r = res.item.ptr;
		r->reference_count--;

		if (r->reference_count != 0) {
			struct key_list *tmp = curr;
			curr = curr->next;
			free(tmp);
			continue;
		}

		if (delete_item(key) != SUCCESS) {
			return FAILURE;
		}

		free(r->region);
		free(r);

		struct key_list *tmp = curr;
		curr = curr->next;
		free(tmp);
	}

	return SUCCESS;
}

enum result_code add_to_scope(struct context *ctx, uint64_t key) {
	if (ctx == nullptr) return FAILURE;
	struct result res = get_item(key);

	if (res.code != SUCCESS) {
		return FAILURE;
	}

	struct region *r = res.item.ptr;
	r->reference_count++;

	add_key(ctx, key);
	return SUCCESS;
}

uint32_t write_to(uint64_t key, void *src, uint32_t size) {
	struct result res = get_item(key);
	if (res.code != SUCCESS) {
		return 0;
	}
	struct region *r = res.item.ptr;
	if (r->size < size) {
		return 0;
	}

	memcpy(r->region, src, size);
	return size;
}

uint32_t read_from(uint64_t key, void *dst, uint32_t size) {
	struct result res = get_item(key);
	if (res.code != SUCCESS) {
		return 0;
	}
	struct region *r = res.item.ptr;
	if (r->size < size) {
		return 0;
	}

	memcpy(dst, r->region, size);
	return size;
}

#define MAIN
#ifdef MAIN
int main(void) {
	init_();

	struct context ctx = {nullptr};
	uint64_t key = 0;
	struct allocation_result res = gc_alloc(&ctx, 23);

	if (res.code != SUCCESS) {
		perror("Allocation failed");
		exit(EXIT_FAILURE);
	}
	key = res.key;
	write_to(key, "CIAO", strlen("CIAO"));

	char buf[23];

	read_from(key, buf, 23);

	printf("%s\n", buf);
	clear_scope(&ctx);

	read_from(key, buf, 23);
	printf("%s\n", buf);

}
#endif