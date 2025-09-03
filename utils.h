#ifndef UTILS_H
#define UTILS_H

struct item {
	uint64_t key;
	struct region *ptr;
};

struct region {
	uint64_t size;
	uint64_t scope_count;
	void *region;
};

enum result_code {
	SUCCESS,
	FAILURE
};

#endif //UTILS_H
