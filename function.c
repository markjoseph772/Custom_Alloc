/* File where the various memory handling functions are defined */
#include <stdio.h>
#include <stdlib.h>
#include "memoryvariables.h"
 /*This function initiallizes the allocator with a customisable pool size  */
 
 int custom_allocator_init(CustomAllocator* allocator, size_t poolSize) {
 	
 	/* Ensure poolsize is large enough to hold at least one Freememorynode */
 	if(poolSize < sizeof(Freememorynode)){
 		printf(" Pool size is to small to be useful.\n");
 		return -1;  /* Indicates an error that poolsize is insufficient */
	 }
	  /* Allocate the memory pool */
	  allocator->tomemorypool = malloc(poolSize);
	  if(!allocator->tomemorypool){
	  	printf("Failed to allocate memory for the pool\n");
	  	return -1; /*Error: memory allocation failed */
	  }
	  /* Set the size of the memorypool i.e poolsize */
	  allocator->poolsize = poolSize;
	  
	  /* Initialize the freememory, starting with the whole memory block as a freememory */
	  allocator->Freememory = (Freememorynode*)allocator->tomemorypool;
	  allocator->Freememory->size = poolSize;
	  allocator->Freememory->next = NULL;
	  
	  printf("Allocator initialized with poolsize:%zu bytes.\n", poolSize);
	  return 0;   /*returns 0 for success */
	  
 }
 
 /* This function allocates memory based on the size requested by the user  */
 
 void* Custom_allocator_alloc(CustomAllocator* allocator, size_t size) {
 	
 	Freememorynode* head = allocator->Freememory;
 	Freememorynode* previous = NULL;
 	
 	 /* Align the requested size to the size of freememorynode to minimise fragmentation */
 	size_t alignedSize = (size + sizeof(Freememorynode)- 1 & ~(sizeof(Freememorynode) - 1)); 
 	
 	/* find a free block that fits the aligned size */
 	while(head!= NULL){
 		if(head->size >= alignedSize){
 			 
 			 /* allocate from this block */
 			 void* allocatedBlock = (void*)head;
 			 size_t remainingSize = head->size - alignedSize;
 			 
 			 /* update the memory that is free to freememory */
 			 if(remainingSize > sizeof(Freememorynode)){
 			 	
 			 	/* split the block if there is enough memory */
 			 	Freememorynode* newFreememory = (Freememorynode*)((char*)head + alignedSize);
 			 	newFreememory->size = remainingSize;
 			 	newFreememory->next = head->next;
 			 	
 			 	 if(previous != NULL){
			  	previous->next = newFreememory;
			  } else {
			  	allocator->Freememory = newFreememory;
			  }
			  } else {
			  	 /* Remove the block from the freelist if it fits exactly */
			  	 if(previous!=NULL){
			  	 	previous->next = head->next;
				   } else{
				   	allocator->Freememory = head->next;
				   }
			  }
			  printf("Allocated %zu bytes.\n",alignedSize);
			  return allocatedBlock;
		 }
		 previous = head;
		 head = head->next;
	 }
	 printf("Out of memory: Unable to allocate %zu bytes.\n", alignedSize);
	 return NULL;      /* out of memory */
 }
 
 void custom_allocator_free(CustomAllocator* allocator, void* ptr){
 	Freememorynode* node1 = (Freememorynode*)ptr;
 	
 	/* Insert the block into the free list */
 	node1->next = allocator->Freememory;
 	allocator->Freememory = node1;
 	
 }
 
 void custom_allocator_reset(CustomAllocator* allocator){
 	
 	allocator->Freememory = (Freememorynode*)allocator->tomemorypool;
 	allocator->Freememory->size = allocator->poolsize;
 	allocator->Freememory->next = NULL;
 }
 
 void defragment_memory (CustomAllocator* allocator){
 	 Freememorynode* head = allocator->Freememory;
 	if(head == NULL) {
 		
 		return; /* no need to defragment if the list is empty */
 		
	 }
	 
	 Freememorynode* current = head;
	 while (current != NULL) {
	 	Freememorynode* next_block = current->next;
	 	
	 	/*check if the current block and the next block are adjacent */
	 	while(next_block != NULL && (char*)current +current->size == (char*)next_block) {
	 		/* We merge the current block to the next block */
	 		current->size +=next_block->size;
	 		current->next =next_block->next;
	 		next_block = current->next;
	 		
		 }
		 
		 /* move to the next block */
		 current = next_block;
	 }
 }
 
 

