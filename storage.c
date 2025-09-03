#include "storage.h"
#include <stdlib.h>
#include <string.h>

#ifndef CACHE_SIZE
#define CACHE_SIZE 64
#endif

struct research_binary_tree {
	struct research_binary_tree *left;
	struct research_binary_tree *right;
	struct item item;
};

struct item *CACHE = nullptr;
uint64_t size;

enum direction {
	LEFT, RIGHT
};

void init_() {
	CACHE = calloc(CACHE_SIZE, sizeof(struct item));
	size = CACHE_SIZE;
}

void init(uint64_t cache_size) {
	CACHE = calloc(cache_size, sizeof(struct item));
	size = cache_size;
}

struct research_binary_tree *root = nullptr;

struct result get_item(uint64_t key) {

	struct result res;

	uint64_t index = key % CACHE_SIZE;
	struct item node = CACHE[index];

	if (node.key == key) {
		res.item = node;
		res.code = SUCCESS;

		return res;
	}

	struct research_binary_tree *current = root;

	while (current != nullptr) {
		if (current->item.key == key) {
			res.item = current->item;
			res.code = SUCCESS;

			CACHE[key%CACHE_SIZE] = res.item;
			return res;
		}

		if (key < current->item.key) {
			current = current->left;
			continue;
		}

		current = current->right;
	}

	res.code = FAILURE;
	memset(&res.item, 0, sizeof(res.item));
	
	return res;
}

enum result_code store_item(struct item item) {
	struct research_binary_tree *node = calloc(1, sizeof(struct research_binary_tree));
	node->item = item;

	if (root == nullptr) {
		root = node;
		return SUCCESS;
	}

	struct research_binary_tree *current = root;
	struct research_binary_tree *parent = nullptr;
	enum direction dir = LEFT;

	while (current != nullptr) {
		if (current->item.key == item.key) {
			return FAILURE;
		}

		if (item.key < current->item.key) {
			parent = current;
			current = current->left;
			dir = LEFT;

			continue;
		}

		parent = current;
		current = current->right;
		dir = RIGHT;
	}

	switch (dir) {
		case LEFT:
			parent->left = node;
			break;
		case RIGHT:
			parent->right = node;
			break;
	}

	return SUCCESS;
}

void swap_tree_nodes(
	struct research_binary_tree *parent,
	struct research_binary_tree *right_child
) {

	struct item temp_item = parent->item;
	parent->item = right_child->item;
	right_child->item = temp_item;
}

enum result_code delete_item(uint64_t key) {
	struct research_binary_tree **debug = &root;

	struct research_binary_tree *curr = root;
	struct research_binary_tree *parent = nullptr;
	enum direction dir = LEFT;

	while (curr != nullptr && curr->item.key != key) {
		if (key < curr->item.key) {
			parent = curr;
			dir = LEFT;
			curr = curr->left;
			continue;
		}

		parent = curr;
		dir = RIGHT;
		curr = curr->right;
	}

	if (curr == nullptr) {
		return FAILURE;
	}

	if (curr->right == nullptr && curr->left == nullptr) {
		if (curr == root) {
			root = nullptr;
			free(curr);

			return SUCCESS;
		}
		switch (dir) {
			case LEFT:
				parent->left = nullptr;
				break;
			case RIGHT:
				parent->right = nullptr;
		}
		free(curr);
		return SUCCESS;
	}

	parent = curr;
	curr = curr->right;

	while (curr->right != nullptr) {
		swap_tree_nodes(parent, curr);
		parent = curr;
		curr = parent->right;
	}

	swap_tree_nodes(parent, curr);

	parent->right = curr->left;
	free(curr);

	return  SUCCESS;
}
