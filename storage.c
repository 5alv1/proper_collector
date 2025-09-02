#include "storage.h"
#include <stdlib.h>

#ifndef CACHE_SIZE
#define CACHE_SIZE 64
#endif

struct research_binary_tree {
	struct research_binary_tree *left;
	struct research_binary_tree *right;
	struct item item;
};

struct research_binary_tree CACHE[CACHE_SIZE] = {nullptr};

enum direction {
	LEFT, RIGHT
};

struct research_binary_tree *root = nullptr;

struct result get_item(uint64_t key) {

	struct result res;

	uint64_t index = key % CACHE_SIZE;
	struct research_binary_tree node = CACHE[index];

	if (node.item.key == key) {
		res.item.key = key;
		res.item.ptr = node.item.ptr;
		res.code = SUCCESS;

		return res;
	}

	struct research_binary_tree *current = root;

	while (current != nullptr) {
		if (current->item.key == key) {
			res.item = current->item;
			res.code = SUCCESS;
			return res;
		}

		if (key < current->item.key) {
			current = current->left;
			continue;
		}

		current = current->right;
	}

	res.code = FAILURE;
	res.item.key = -1;
	res.item.ptr = nullptr;

	return res;
}

enum result_code store_item(struct item item) {
	struct research_binary_tree *node = calloc(1, sizeof(struct research_binary_tree));
	node->item = item;

	if (nullptr == node) {
		return FAILURE;
	}

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

	struct research_binary_tree *curr = root;
	struct research_binary_tree *parent = nullptr;

	while (curr != nullptr && curr->item.key != key) {
		if (key < curr->item.key) {
			curr = curr->left;
			continue;
		}

		curr = curr->right;
	}

	if (curr == nullptr) {
		return FAILURE;
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
