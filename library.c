#include "library.h"
#include "storage.h"

#include <stdio.h>

void hello(void) {
	printf("Hello, World!\n");
}

#ifdef MAIN
int main(void) {
	hello();

	struct item item;
	item.ptr = nullptr;
	item.key = 69;

	store_item(item);

	item.key = 75;
	store_item(item);

	item.key = 72;
	store_item(item);

	item.key = 76;
	store_item(item);

	delete_item(69);
}
#endif