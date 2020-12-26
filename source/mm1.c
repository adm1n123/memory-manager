/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "segfault_11",
    /* First member's full name */
    "Deepak Singh Baghel",
    /* First member's email address */
    "deepakbaghel@cse.iitb.ac.in",
    /* Second member's full name (leave blank if none) */
    "Abhinandan Singh",
    /* Second member's email address (leave blank if none) */
    "abhinandan@cse.iitb.ac.in"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 
 * mm_init - initialize the malloc package.
 */

void *init_mem_sbrk_break = NULL;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ My lib +++++++++++++++++++++++++++++++++++++++++++++++++++

char *last_block;
#define HEADER_SIZE 16
#define INT_MAX 2147483647
char *create_header(char *start_addr, int block_size, int status, char *pre_block) {
	// block_size is total size including header
	int *ptr = (int *)start_addr;
	*ptr = block_size;
	ptr += 1;
	*ptr = status;
	ptr += 1;
	// block_ptr is variable in side method it has value which is address of header
	char **block_ptr = (char **)ptr;
	*block_ptr = pre_block;
	return start_addr + HEADER_SIZE; // return body pointer
}

int get_block_size(char *header) {
	int *ptr = (int *)header;
	return *ptr;
}
int get_body_size(char *header) {
	int *ptr = (int *)header;
	return *ptr - HEADER_SIZE;
}
int get_block_status(char *header) {
	int *ptr = (int *)header;
	ptr += 1;
	return *ptr;
}
char *get_pre_header(char *header) {
	int *ptr = (int *)header;
	ptr += 2;
	char **block_ptr = (char **)ptr;
	if(*block_ptr < (char *)mem_heap_lo()) return (char *)-1;
	return *block_ptr;
}
void set_block_size(char *header, int size) {
	int *ptr = (int *)header;
	*ptr = size;
	return;
}
void set_block_status(char *header, int status) {
	int *ptr = (int *)header;
	ptr += 1;
	*ptr = status;
	return;
}
char *get_header_pointer(char *body_pointer) {
	return body_pointer - HEADER_SIZE;
}
char *get_body_pointer(char *header) {
	return header + HEADER_SIZE;
}

char *get_next_header(char *header) {
	char *my_mem_brk = (char *)mem_heap_hi()+1;
	if(header + HEADER_SIZE >= my_mem_brk) return (char *)-1; // == is required only because header is always valid but >= just for safety
	header += get_block_size(header);
	if(header + HEADER_SIZE > my_mem_brk) return (char *)-1; // it could be empty block hence > instead of >=
	return header;
}
void set_next_block_pre_pointer(char *header) { // next block must exist.
	char *next_header = get_next_header(header);
	int *ptr = (int *)next_header;
	ptr += 2;
	char **block_ptr = (char **)ptr;
	*block_ptr = header;
	return;
}

void print_header(char *header) {
	printf("Body size: %d	Block status: %d\n", get_body_size(header), get_block_status(header));
}
void print_list(char *header) {
	printf("printing list ----------------------------------\n");
	while(1) {
		print_header(header);
		header = get_next_header(header);
		if(header == (char *)-1) break;
		if(get_block_size(header) < HEADER_SIZE) {
			printf("error block size can not be less than header size\n");
			exit(1);
		}
		
	}
	printf("list printed ++++++++++++++++++++\n");
}
void print_rev_list(char *last_block) {
	char *header = last_block;
	printf("printing reverse list ---------------------------\n");
	while(1) {
		print_header(header);
		header = get_pre_header(header);
		if(header == (char *)-1) break;
		if(get_block_size(header) < HEADER_SIZE) {
			printf("error block size can not be less than header size\n");
			exit(1);
		}
		
	}
	printf("list printed +++++++++++++++++++\n");
}

char *coalesce_next(char *header) { // passed header must be empty
	char *next_header = get_next_header(header);
	if(next_header != (char *)-1) {
		int status_next = get_block_status(next_header);
		if(status_next == 0) {
			int block_size = get_block_size(header);
			int next_block_size = get_block_size(next_header);
			set_block_size(header, block_size + next_block_size);
			if(next_header == last_block) {
				last_block = header;
			} else {
				set_next_block_pre_pointer(header);
			}
			return header;
		}
	}
	return (char *)-1;
}
void coalesce(char *header) { // passed header must be empty block
	coalesce_next(header);
	char *pre_header = get_pre_header(header);
	if(pre_header == (char *)-1) return;
	int status = get_block_status(pre_header);
	if(status == 0) coalesce_next(pre_header);
}

char *get_best_block(char *start_from, size_t body_size) {
	char *my_mem_brk = (char *)mem_heap_hi()+1;
	char *header_ptr = start_from;
	char *end_ptr = my_mem_brk - HEADER_SIZE;
	char *best_block_ptr = (char *)-1;
	int best_body_size = INT_MAX;
	int tstatus, tsize;

	while (header_ptr <= end_ptr) {
		tstatus = get_block_status(header_ptr);
		if(tstatus == 0) { // if block is empty
			tsize = get_body_size(header_ptr);
			if(tsize >= body_size && tsize < best_body_size) {
				best_body_size = tsize;
				best_block_ptr = header_ptr;
			}
		}
		header_ptr = get_next_header(header_ptr);
		if(header_ptr == (char *)-1) break;
	}
	return best_block_ptr;
}
int allocate_block(char *header, int body_size) {

	int block_size = get_block_size(header);
	int new_block_size = body_size + HEADER_SIZE;
	int next_block_size = block_size - new_block_size;

	if(next_block_size < HEADER_SIZE) {
		set_block_status(header, 1); // occupied
		return 1;
	}
	set_block_status(header, 1); // occupied
	set_block_size(header, new_block_size);

	char *next_block_header = header + new_block_size;
	create_header(next_block_header, next_block_size, 0, header);
	
	if(header == last_block) {
		last_block = next_block_header;
	} else {
		set_next_block_pre_pointer(next_block_header);
	}

	return 1;
}
char *realloc_in_place(char *header, int new_body_size) {

	int block_size = get_block_size(header);
	int new_block_size = new_body_size + HEADER_SIZE;
	if(block_size == new_block_size) return header;
	if(block_size > new_block_size) {
		int next_block_size = block_size - new_block_size;

		if(next_block_size < HEADER_SIZE) {
			return header;
		}		
		set_block_size(header, new_block_size);

		char *next_block_header = header + new_block_size;
		create_header(next_block_header, next_block_size, 0, header);

		if(header == last_block) {
			last_block = next_block_header;
		} else {
			set_next_block_pre_pointer(next_block_header);
		}
		return header;
	}
	return (char *)-1;
}
char *copy_body(char *to, char *from) {
	int body_size = get_body_size(from);
	memcpy(to + HEADER_SIZE, from + HEADER_SIZE, (size_t)((unsigned)body_size));
	return to;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ It was my Lib +++++++++++++++++++++++++++++++++++++++++++++++++



int mm_init(void) {
	last_block = NULL;
    return 0;		//Returns 0 on successfull initialization.
}

void *mm_malloc(size_t size) {	

	if(size <= 0){		// Invalid request size
		return NULL;
	}
	if(mem_heapsize() >= HEADER_SIZE) {	// find best block
		int align_size = ALIGN(size);
		char *header = get_best_block(mem_heap_lo(), align_size);
		if(header != (char *)-1) {
			allocate_block(header, align_size);
			return (void *)get_body_pointer(header);
		}
	}
	
	int block_size = size + HEADER_SIZE; // total size
	block_size = ALIGN(block_size);
	char *header = mem_sbrk(block_size);		//mem_sbrk() is wrapper function for the sbrk() system call. 
												
	create_header(header, block_size, 1, last_block);
	last_block = header;
	return (void *)get_body_pointer(header);
}


void mm_free(void *ptr) {
	char *header = get_header_pointer((char *)ptr);
	set_block_status(header, 0);
	coalesce(header);
}

void *mm_realloc(void *ptr, size_t size) {

	if(ptr == NULL){			//memory was not previously allocated
		return mm_malloc(size);
	}
	if(size == 0){				//new size is zero
		mm_free(ptr);
		return NULL;
	}
	
	size = ALIGN(size);
	char *old_header = get_header_pointer((char *)ptr);
	char *new_header = realloc_in_place(old_header, size);
	if(new_header != (char *)-1) {
		return (void *)get_body_pointer(new_header);
	}
	char *new_body_pointer = (char *)mm_malloc(size);
	new_header = get_header_pointer(new_body_pointer);

	copy_body(new_header, old_header);
	mm_free((void *)get_body_pointer(old_header));
	return (void *)get_body_pointer(new_header);
}














