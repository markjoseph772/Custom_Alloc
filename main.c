#include <stdio.h>
#include <stdlib.h>
#include "memoryvariables.h"

 extern int custom_allocator_init(CustomAllocator* allocator, size_t poolSize);
 extern void* Custom_allocator_alloc(CustomAllocator *allocator, size_t size);
 extern void custom_allocator_free(CustomAllocator* allocator, void* ptr);
 extern void custom_allocator_reset(CustomAllocator* allocator);
 extern void defragment_memory();
 int main() {
 	
 	CustomAllocator Def_allocator;
 	size_t poolSize;
 	
 	/* prompt the user to enter the size of memory pool */
 	printf("Enter the memory pool size (in bytes):");
 	scanf("%zu", &poolSize);
 	
 	custom_allocator_init(&Def_allocator, poolSize);
 	
 	void* ptr1 = Custom_allocator_alloc(&Def_allocator, 100);
 	void* ptr2 = Custom_allocator_alloc(&Def_allocator, 200);
 	defragment_memory();
 	
 	custom_allocator_free(&Def_allocator, ptr1);
 	custom_allocator_free(&Def_allocator, ptr2);
 	
 	custom_allocator_reset(&Def_allocator);
 	
 	free(Def_allocator.tomemorypool);
 	return 0;
 } 
