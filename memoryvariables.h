 /* headerfile containing all the structure variables */
 typedef struct Freememorynode{
 	
 	size_t size;
 	struct Freememorynode *next;
 } Freememorynode;
 
 typedef struct CustomAllocator{
 	
 	void* tomemorypool;
 	size_t poolsize;
 	Freememorynode* Freememory;
 }CustomAllocator;
 
