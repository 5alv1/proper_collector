//
// Created by Cristian Salvi on 02/09/25.
//

#ifndef STORAGE_H
#define STORAGE_H
#include <stdint.h>

#include "collector.h"
#include "utils.h"

struct result {
	enum result_code code;
	struct item item;
};

struct result get_item(uint64_t key);

enum result_code store_item(struct item item);

enum result_code delete_item(uint64_t key);

#endif //STORAGE_H
