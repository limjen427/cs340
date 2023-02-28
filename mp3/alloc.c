/**
 * Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *startOfHeap = NULL;

typedef struct _metadata_t {
  unsigned int size;     // The size of the memory block.
  unsigned char isUsed;  // 0 if the block is free; 1 if the block is used.
} metadata_t;

void *head_ptr = NULL;
/**
 * When memory is re-used, it is very easy to simple mark the block isUsed to be 1 again even if the block is not a perfect fit. 
 * Could you split a block if there’s enough space left over to leave some free space for a future allocation?
 * 
 * @param ptr
 *    ptr of block that we will split
 * @param size
 *    amount of size that we want to split and the size of block that we will use
 * 
 * 
 * @return 
 *   ptr that ptr should be stored
 */


void *block_split(void *ptr, size_t size) {
  //printf("-- start of the block_split function --\n");
  //printf("start ptr: %p\n", ptr);
  metadata_t *block = ptr - sizeof(metadata_t);
  metadata_t *left = ptr + size;
  left->size = block->size - (sizeof(metadata_t) + size);

  //printf("block->size: %d, size: %ld\n", block->size, size);
  if (left->size > sizeof(metadata_t)) {
    left->isUsed = 0;
    //printf("metadata left for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)left + sizeof(metadata_t), left, left->size, left->isUsed);

    block->size = size;
    block->isUsed = 1;
    //printf("metadata block(will use) for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)block + sizeof(metadata_t), block, block->size, block->isUsed);

    //printf("check if the ptr is right one: %p\n", ptr);
    //printf("-- end of the block_split function --\n");
  }
  void *temp_ptr = block + sizeof(metadata_t);
  return temp_ptr;
}


/*
Currently, when memory is freed, 
two blocks of free memory may appear next to one-another. 
In our example, this happened with a and b. 
Could you combine the blocks together to create one larger block?

With successful memory coalescing, 
you should see the free block of a and b 
in the sample1 program join together 
for a free block of over 500 bytes.

*/
/**
 * @param size
 *  size we want to allocate. Need to check if the size is enough with "size" if we merge two free block
 * @param ptr
 *   ptr that we would like to malloc it
 * 
 * @return 
 *   ptr that ptr should be stored
*/
void *block_merge(void* ptr, size_t size) {
  //printf("-- start of the block_merge function --\n");
  //printf("start ptr: %p\n", ptr);

  metadata_t *cur = ptr - sizeof(metadata_t);   //store first block
  metadata_t *next = ptr + cur->size;
  void* use_ptr = NULL;
  size_t max_size = cur->size;

  //printf("metadata cur for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)cur + sizeof(metadata_t), cur, cur->size, cur->isUsed);
  //printf("metadata temp for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)next + sizeof(metadata_t), next, next->size, next->isUsed);


  while (next->isUsed == 0 && next->size != 0) {
    //printf("next is not NULL\n");
    metadata_t *temp = next;
    max_size = max_size + sizeof(metadata_t) + temp->size;
    next = (void *)next + next->size + sizeof(metadata_t);
  }

  cur->size = max_size;
  //printf("new cur->size: %d\n", cur->size);
  cur->isUsed = 1;
  void *cur_ptr = cur + sizeof(metadata_t);
  //printf("ptr for cur: %p\n", ptr);
  use_ptr = block_split(cur_ptr, size);
  //printf("same ptr as above?: %p\n", use_ptr);


  //printf("-- end of the block_merge function --\n");
  return use_ptr;
}


/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */

void *malloc(size_t size) {
  //printf("-- start of the malloc function --\n");
  void *malloc_ptr = NULL;
  // Allocate heap memory for the metadata structure and populate the variables:
  if (size == 0) {return NULL;}

  //printf("Inside: malloc(%lu):\n", size);
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);
  }
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  //printf("-- Start of Heap (%p) --\n", startOfHeap);
  if (head_ptr != NULL) {
    while ((void *)curMeta < endOfHeap) {   // While we're before the end of the heap...
    //printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);

    if (curMeta->isUsed == 0) {
      void *temp_ptr = (void *)curMeta + sizeof(metadata_t);

      if (curMeta->size == size) {
        curMeta->isUsed = 1;
        return temp_ptr;
      }
      else if (curMeta->size > size) {
        malloc_ptr = block_split(temp_ptr, size);
        metadata_t *temp_meta = malloc_ptr - sizeof(metadata_t);
        temp_meta->size = size;
        temp_meta->isUsed = 1;
        return malloc_ptr;
      }
      else if (curMeta->size < size) {
        if (curMeta->size == 0) {break;}
        malloc_ptr = block_merge(temp_ptr, size);
        if (malloc_ptr != NULL) {
          metadata_t *temp_meta = malloc_ptr - sizeof(metadata_t);
          temp_meta->size = size;
          temp_meta->isUsed = 1;
          return malloc_ptr;
        }
      }
    }
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
    }
  }
  
  //printf("-- End of Heap (%p) --\n\n", endOfHeap);


  //if no free block is available, just add new block
  metadata_t *meta = sbrk( sizeof(metadata_t) );
  meta->size = size;
  meta->isUsed = 1;

  // Allocate heap memory for the requested memory:
  //printf("Reach to the sbrk for malloc\n");
  malloc_ptr = sbrk( size );

  //printf("-- end of the malloc function --\n");
  // Return the pointer for the requested memory:
  return malloc_ptr;
}


/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void *ptr) {
  // Find the metadata located immediately before `ptr`:
  if (ptr == NULL) {return;}

  metadata_t *meta = ptr - sizeof( metadata_t );
  if (meta == NULL) {return;}
  
  // Mark the allocation is free:
  meta->isUsed = 0;

  head_ptr = sbrk(0);
}

/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
  //printf("-- start of the realloc function --\n");
  void *realloc_ptr = ptr;

  //if ptr == NULL
  if (ptr == NULL) {
    realloc_ptr = malloc(size);
    return realloc_ptr;
  }

  //if size == 0
  if (size == 0) {
    //deallocate previous ptr and return NULL
    free(ptr);
    return NULL;
  }

  //from startOfHeap
  //printf("Inside: realloc(%lu):\n", size);
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);
  }
  metadata_t *curMeta = startOfHeap;
  //printf("-- Start of Heap (%p) --\n", startOfHeap);
  while ((void *)curMeta) {
    // find the metadata that isUsed == 0 (free block)
    if (curMeta->isUsed == 0) {
      //printf("find the free block %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
      //ptr value for curMeta
      void *temp_ptr = (void *)curMeta + sizeof(metadata_t);
      //printf("temp_ptr: %p\n", temp_ptr);
      //printf("curMeta->size=%d, size=%ld\n", curMeta->size, size);
      //compare that block size and "size"
      //block sizes are exactly same, no need to split or merge
      if (curMeta->size == size) {
        //printf("curMeta->size == size: ptr?: %p\n", (void *)curMeta + sizeof(metadata_t));
        realloc_ptr = curMeta + sizeof(metadata_t);
        //printf("success on finding ptr %p\n", realloc_ptr);
        memcpy(realloc_ptr, ptr, size);
        free(ptr);
        return realloc_ptr;
      }
      //block size > size, split
      else if (curMeta->size > size) {
        //printf("curMeta->size > size, ptr: %p\n", (void *)curMeta + sizeof(metadata_t));
        realloc_ptr = block_split(temp_ptr, size);
        //printf("success on finding ptr: %p, need to be same as above.\n", realloc_ptr);
        memcpy(realloc_ptr, ptr, size);
        //printf("success memcpy\n");
        free(ptr);
        return realloc_ptr;
      }
      //block size < size, merge
      else {
        //printf("curMeta->size < size, ptr: %p\n", (void *)curMeta + sizeof(metadata_t));
        if (curMeta->size == 0) {break;}
        realloc_ptr = block_merge(temp_ptr, size);
        if (realloc_ptr != NULL) {
          //printf("success on finding ptr: %p, need to be same as above.\n", realloc_ptr);
          memcpy(realloc_ptr, ptr, size);
          //printf("success memcpy\n");
          free(ptr);
          return realloc_ptr;
        } else {break;}
      }
    }
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  }
  //printf("-- End of Heap (%p) --\n\n", (void *)curMeta);

  //free ptr(that is given in parameter)
  //free(ptr);
  //printf("-- end of the malloc function --\n");
  //return the ptr value that data is stored
  return realloc_ptr;
}

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
  //printf("-- Start of the calloc function --\n");
  void *calloc_ptr = NULL;
  unsigned int calloc_size = num * size;
  
  // returns NULL if num or size are 0 
  if (num == 0 || size == 0) {return NULL;}
  
  //if the num*size would result in interger overflow, returns an error
  //if (calloc_size > 2147483647 || calloc_size < -2147483648) {return NULL;}
  

  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);
  }
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  //printf("-- Start of Heap (%p) --\n", startOfHeap);
  while ((void *)curMeta < endOfHeap) {   // While we're before the end of the heap...
    //printf("metadata for curMeta %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    if (curMeta->isUsed == 0) {
      void *temp_ptr = (void *)curMeta + sizeof(metadata_t);
      //printf("metadata curMeta for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
      if (curMeta->size == calloc_size) {
        //printf("curMeta->size:%d == calloc_size: %d\n", curMeta->size, calloc_size);
        curMeta->isUsed = 1;
        if (temp_ptr == NULL) {return NULL;}
        memset(temp_ptr, 0, calloc_size);
        //printf("finish memset for same size\n");
        return temp_ptr;
      }
      else if (curMeta->size > calloc_size) {
        //printf("curMeta->size: %d > calloc_size : %d\n", curMeta->size, calloc_size);
        calloc_ptr = block_split(temp_ptr, calloc_size);
        //printf("block_split pass. calloc_ptr: %p\n", calloc_ptr);
        metadata_t *temp_meta = calloc_ptr - sizeof(metadata_t);
        temp_meta->size = calloc_size;
        temp_meta->isUsed = 1;
        if (calloc_ptr == NULL) {return NULL;}
        memset(calloc_ptr, 0, calloc_size);
        //printf("finish memset for curMeta->size > calloc_size\n");
        return calloc_ptr;
      }
      else if (curMeta->size < calloc_size) {
        //printf("curMeta->size: %d < calloc_size : %d\n", curMeta->size, calloc_size);
        if (curMeta->size == 0) {break;}
        if (((void *)curMeta + sizeof(metadata_t) + curMeta->size) >= endOfHeap) {
          //printf("next of curMeta >= endOfHeap check.\n");
          break;
        }
        //printf("curMeta->size == 0 case pass.\n");
        calloc_ptr = block_merge(temp_ptr, calloc_size);
        //printf("block_merge pass. calloc_ptr: %p\n", calloc_ptr);
        if (calloc_ptr != NULL) {
          //printf("if calloc_ptr != NULL\n");
          metadata_t *temp_meta = calloc_ptr - sizeof(metadata_t);
          temp_meta->size = calloc_size;
          temp_meta->isUsed = 1;
          if (calloc_ptr == NULL) {return NULL;}
          memset(calloc_ptr, 0, calloc_size);
          //printf("finish memset for curMeta->size > calloc_size\n");
          return calloc_ptr;
        } 
        else {
          //printf("calloc_ptr == NULL\n");
          break;
        }
      }
    }
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  }
  //printf("-- End of Heap --\n");

  metadata_t *meta = sbrk(sizeof(metadata_t));
  meta->size = calloc_size;
  meta->isUsed = 1;

  // allocate memory blcok
  calloc_ptr = sbrk(calloc_size);
  
  // initializes all bytes to 0
  memset(calloc_ptr, 0, calloc_size);
  
  
  //printf("-- End of the calloc function --\n");
  // Return the pointer for the requested memory:
  return calloc_ptr;

}


