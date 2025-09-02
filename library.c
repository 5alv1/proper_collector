#include "library.h"

#include <stdio.h>

void hello(void) {
	printf("Hello, World!\n");
}

#ifdef MAIN
int main(void) {
	hello();
}
#endif