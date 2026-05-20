/*Connor Riley
4/7/24
malloc lab cs 360
Recreating the malloc() and free() call using sbrk and singly linked lists
that mimic the free list 
*/
#include "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  size_t size;
  struct chunk *flink;
} Chunk;

struct chunk *head = NULL;

/*main function of malloc, gives the user a memory address that will store the
required size of the data with an extra 8 for book keeping*/
void *my_malloc(size_t size) {
  if (size <= 0) {
    // invalid size used
    return NULL;
  }
  // padding algorithm to round it to 8 bytes
  size_t chunk_padding = (size + 7 + 8) & -8;
  size_t min_sbrk = 8192;         // set mininum sbrk call allowed
  size_t size_of_sbrk = min_sbrk; // set current sbrk size to min

  if (chunk_padding > min_sbrk) {
    size_of_sbrk = chunk_padding; // if need more space then 8192
  }
  // if head hasnt been set then we allocate
  // 8192 bytes and set the head equal to the head
  if (head == NULL) {
    head = (struct chunk *)sbrk(size_of_sbrk);
    head->size = size_of_sbrk;
    head->flink = NULL;
  }
  // set starting pointer to the head
  // iterate until you find a chunk that has enough space in the size
  struct chunk *chunk_ptr = head;
  while (chunk_ptr->size < chunk_padding) {
    if (chunk_ptr->flink != NULL) {
      chunk_ptr = chunk_ptr->flink; // find end of used chunks
    } else {
      chunk_ptr->flink = (struct chunk *)sbrk(size_of_sbrk);
      chunk_ptr = chunk_ptr->flink;
      chunk_ptr->flink = NULL;
      chunk_ptr->size = size_of_sbrk;
    }
  }
  // if the size is large enough then we have a memory location for the chunk
  if (chunk_ptr->size >= chunk_padding + 8) {
    char *mem_location = (char *)chunk_ptr;
    mem_location = mem_location + chunk_ptr->size;
    mem_location = mem_location - chunk_padding;
    *((int *)mem_location) = chunk_padding;
    mem_location = mem_location + 8;
    chunk_ptr->size = chunk_ptr->size - chunk_padding;
    return (void *)mem_location; // return the memory location
  } else {
    *((int *)chunk_ptr) = chunk_padding;
    if (chunk_ptr == head) {
      head = head->flink;
    } else {
      struct chunk *tmp = head;
      while (tmp->flink != chunk_ptr) {
        tmp = tmp->flink;
      }
      tmp->flink = tmp->flink->flink;
    }
    return ((void *)chunk_ptr) + 8; // return memory location
  }
}

// free space of the desired chunk passed to the function
void my_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  struct chunk *tmp = head;
  head = ptr - 8;
  head->flink = tmp;
}

void *free_list_begin() {
  return head; // get the head of the list
}

// get the flink of the passed node
void *free_list_next(void *node) {
  if (node == NULL) {
    return NULL;
  }
  return ((struct chunk *)node)->flink;
}
// concatnate the free list
// find the amount of used chunks and iterate through them
// sort the list using 2 for loops bc time dont matter rn and i
// have never used qsort and i dont want to now
void coalesce_free_list() {
  int chunk_count = 0;
  struct chunk *chunk_ptr = free_list_begin();
  while (chunk_ptr != NULL) {
    chunk_count += 1;
    chunk_ptr = chunk_ptr->flink;
  }
  // have two sets of chunks, unsorted and sorted
  int chunks[chunk_count];
  int sorted_chunks[chunk_count];
  chunk_ptr = free_list_begin();

  for (int i = 0; i < chunk_count; i++) {
    chunks[i] = (unsigned int)chunk_ptr;
    chunk_ptr = chunk_ptr->flink;
  }

  unsigned int min = chunks[0];
  unsigned int index = 0;
  // bubble sort
  for (int i = 0; i < chunk_count; i++) {
    for (int j = 0; j < chunk_count; j++) {
      if (chunks[j] <= min) {
        min = chunks[j];
        index = j;
      }
    }
    chunks[index] = -1;
    sorted_chunks[i] = min;
    min = -1; // its unsigned so it actually is very large, this way
    // it cant be the min value again

    if (i != 0) {
      ((struct chunk *)sorted_chunks[i - 1])->flink = (struct chunk *)sorted_chunks[i];
    }
  }
  //set the head to first of the sorted list
  head = (struct chunk *)sorted_chunks[0];
  ((struct chunk *)sorted_chunks[chunk_count - 1])->flink = NULL;//set last flink to null
  chunk_ptr = free_list_begin();//go back to beginning

  struct chunk *tmp;
  while (chunk_ptr != NULL) {
    if (chunk_ptr->size == 0) {
      chunk_ptr = chunk_ptr->flink;
      continue;
    }
    if (chunk_ptr->flink == NULL) {
      chunk_ptr = chunk_ptr->flink;
      continue;
    }
    //check if the size of the chunk + the pointer address is equal to the
    //flink as this will mean that the list has been compacted as much as possible
    tmp = chunk_ptr->flink;

    while (((void *)chunk_ptr) + chunk_ptr->size == tmp) {
      chunk_ptr->flink = tmp->flink;
      chunk_ptr->size += tmp->size;
      tmp->size = 0;
      tmp = tmp->flink;
    }
    //next chunk time
    chunk_ptr = chunk_ptr->flink;
  }
}
