#ifndef PROPER_COLLECTOR_LIBRARY_H
#define PROPER_COLLECTOR_LIBRARY_H

#include <stdlib.h>
#include <stdint.h>

#include "utils.h"

struct key_list {
	uint64_t key;
	struct key_list *next;
};

struct context {
	struct key_list *keys;
};

struct allocation_result {
	enum result_code code;
	uint64_t key;
};
struct allocation_result gc_alloc(struct context *ctx, uint32_t size_);
enum result_code clear_scope(struct context *ctx);
enum result_code add_to_scope(struct context *ctx, uint64_t key);

uint32_t write_to(uint64_t key, void *src, uint32_t size);
uint32_t read_from(uint64_t key, void *dst, uint32_t size);

#endif //PROPER_COLLECTOR_LIBRARY_H