/*
	
	mm2.c
	
	**************************************************************************************************************************************************
	**************************************************************************************************************************************************
	Design explanation: 

			In this approach an implicit doubly linked list is used to keep track of all the block and AVL Tree is used to keep track of
			only free block unlike linked list used in mm1.c to keep track of free block.
				
			N: be the number of free blocks.
			M: be the number of free blocks with distinct size. 
			M <= N
	
								Search Time 		Deletion Time		Insertion Time
			
				Linked list:			O(N)			O(1)			O(1)
					
				AVL Tree			O(log(M))		O(1)			O(log(M))
				(sibling node)

				AVL Tree			O(log(M))		O(log(M))		O(log(M))
				(non-sibling node)

						

			Since AVL Tree gives consistent performance for all the operations hence it is better than linked list for widest veriety of 
			loads In case when number of free bloks are very high then linked list fail to perform but AVL Tree perform consistently.

		Sibling concept improved performance:
			Number of nodes in AVL Tree are less than or equals to the number of free block. All the free blocks with equal size are being 
			stored in same node of AVL Tree in doubly linked list hence when ever a block is searched then pointer to second sibling is 
			returned in this way when we delete the second sibling since it is going to be allocated then deletion operation is done in 
			constant time.

		Two way Coalescing:
			when any block is freed the two way coalescing is done. to ensure no two adjacent blocks are free blocks.


	***************************************************************************************************************************************************
	***************************************************************************************************************************************************

	Correctness:

		Initially doubly linked list of all the blocks and AVL Tree are empty when new block is allocated then doubly linked list is initailized
		and subsequent allocation of blocks are being inserted in doubly linked list. when block is freed then AVL Tree root is initialized with
		free block and subsequent free bloks are inserted into AVL Tree. when a block is reallocated then it is resized inplace if next block is
		free otherwise it is relocated to existing free block or new block is created if there is not any best fit block. if all the free blocks
		are used for allocation then root of AVL Tree is set to NULL.
			
		mm_malloc:
			first search in the AVL Tree for best fit block if found then allocate the space set the header fields. and create the new free block 
			if space is left after allocation. and delete the free block from AVL Tree.I f not found then use mem_sbrk to allocate new block at 
			the end of manageable space.

		mm_free:
			when a block is freed then set the status = 0 and then do two way coalescing and delete the blocks from AVL Tree if they are being 
			coalesced. Then insert free block into AVL Tree which is formed by coalescing.
		mm_realloc:
			when a block is reallocated then it is resized inplace if next block is free and have required space otherwise malloc is use for new
			space allocation followed by mm_free to free the old block and insert into AVL Tree. when block is at the end of heap and there are
			no best fit free block available then mm_realloc call mem_sbrk method for the extra size which is required for block and block is 
			extenden inplace.

		1. All the blocks which are stored in doubly linked are the only blocks which are present in manageable space there is no such block of 
		   memory which is not in the doubly linked list at any instance. whenever a new block is created its header is inserted in the doubly
		   linked list with help of previous block pointer and next block pointer.
		2. All the blocks which are stored in AVL Tree are the free blocks with status = 0. which is set when a block is freed or new block is 
		   created then it is inserted into AVL Tree.
		3. All the blocks which are stored in AVL Tree has valid parent pointer, left child pointer, right child pointer and sibling pointer which
		   is set when block is inserted and blocks which are not free with status = 0 has all the field NULL which is set when a block from AVL
		   Tree is deleted and used for allocation.
		4. Height of all the blocks which are not in AVL Tree is set to zero when block is deleted from AVL Tree.
		5. All the free blocks which have same block size as any other free blocks have valid Left Sibling Pointer and Right Sibling Pointer.
		   blocks which are not free and blocks which are free but with unique size have both Left Sibling Pointer and Right Sibling Pointer as
		   NULL which ensures the consitency.


	***************************************************************************************************************************************************
	***************************************************************************************************************************************************




	In this approach a single block is divided into 2 parts.
	+-------------+
	| Header Part |
	+-------------+
	| Data Part   |
	+-------------+

	The Header Part contains 11 entries
	+------+-----------+--------+--------+-----------------------+--------------------+----------------+--------------------+---------------------+----------------------+-----------------------+
	| Size | Data Size | Status | Height | Previos Block Pointer | Next Block Pointer | Parent Pointer | Left Child Pointer | Right Child Pointer | Left Sibling Pointer | Right Sibling Pointer |
	+------+-----------+--------+--------+-----------------------+--------------------+----------------+--------------------+---------------------+----------------------+-----------------------+
	
	Status and Height are 2 bytes each and rest entries in header are 4 bytes each. So header size is 40 bytes(for 32 bit machine).

	Size - total size of block (header + data)

	Data Size - requsted size by user for the block (does not include header and extra allocation because of implementation)
				(0 if block is free)

	Status - status of block 0 if block is free, 1 if block is not free.
			(all the block stored in AVL tree has status 0)

	Height - height of free block stored in AVL tree.
			(0 if block is leaf node, 0 if block is not free, 0 if block is not first sibling)
	
	Previos Block Pointer - Points to the block which present just before this block 
							(NULL if this block is first block)
	
	Next Block Pointer - Points to the block which present just after this block 
						(NULL if this block is last block)

	Parent Pointer-		Points to parent block stored in AVL tree
						(NULL if it is root of tree, NULL if block is not free)
	
	Left Child Pointer-	Points to left child in AVL tree 
						(NULL if it has no left child, NULL if block is not free)

	Right Child Pointer-Points to right child in AVL tree 
						(NULL if it has no right child, NULL if block is not free)

	
	### Concept if Siblings: all the free blocks with same Size are called siblings and stored in same node of AVL tree only first sibling has pointer 
							to parent, pointer to left child and pointer to right child. rest of the siblings has NULL values for parent, left child 
							and right child. Siblings are stored in doubly linked list.

	Left Sibling Pointer-	Points to left sibling stored in same node in AVL tree node
							(NULL if it is first sibling, NULL if block is not free)

	Right Sibling Pointer-	Points to right sibling in AVL tree node
							(NULL if it is last sibling child, NULL if block is not free)

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




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++   Managing Doubly Linked list of All the blocks   ++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Maximum Size of Integer 32 bits size */
#define INT_MAX 2147483647

/* Size of Block header */
#define HEADER_SIZE (ALIGN(sizeof(struct header)))

/* Root of AVL tree */
struct header *root;

/* Pointer to very first block's header */
struct header *first_header;

/* Pointer to last block's header */
struct header *last_header;

void avl_delete(struct header *node);
void avl_insert(struct header *new_node);

/* Structure of Block's header */
struct header {
	size_t size;
	size_t data_size;
	short int status;
	short int height;
	struct header *prev_header;
	struct header *next_header;
	struct header *parent;
	struct header *left;
	struct header *right;
	struct header *left_sibling;
	struct header *right_sibling;
};

/* 
	Parameters: block header pointer, size, data size, status, 
				previous block pointer, next block pointer;

	intialize value of header fields when new block is created 

	returns:
	
*/
void create_header(struct header *header, size_t size, size_t data_size, short int status, 
		struct header *prev_header, struct header *next_header) {

	header->size = size;
	header->data_size = data_size;
	header->status = status;
	header->height = 0;
	header->prev_header = prev_header;
	header->next_header = next_header;
	header->parent = NULL;
	header->left = NULL;
	header->right = NULL;
	header->left_sibling = NULL;
	header->right_sibling = NULL;
	return;
}

/* 
	Parameters: block header pointer, size, data size, status;
	
	reset the size, data size and status field 

	returns:
	
*/
void init_header(struct header *header, size_t size, size_t data_size, short int status) {

	header->size = size;
	header->data_size = data_size;
	header->status = status;
	return;
}

/* 
	Parameters: block header pointer;
	
	when node is deleted from AVL tree header is reset to nullify the fields which are used in AVL tree 

	returns:
	
*/
void reset_header(struct header *header) {

	header->height = 0;
	header->parent = NULL;
	header->left = NULL;
	header->right = NULL;
	header->left_sibling = NULL;
	header->right_sibling = NULL;
	return;
}

/*
	Parameters: pointer to header part of block

	returns: the Pointer to Data part of block
*/
char *get_body_pointer(struct header *header) {

	return (char *)header + HEADER_SIZE;
}

/*
	Parameters: pointer to data part of block

	returns: the Pointer to header part of block
*/
struct header *get_header_pointer(char *body_ptr) {

	return (struct header *)(body_ptr - HEADER_SIZE);
}

/*
	Parameters: pointer to header part of block
	
	coalesces the block with next block it is guarantee that next block is free

	returns: pointer to header part of block after coalesce which is same.
*/
struct header *coalesce_next(struct header *header) {

	struct header *next_header = header->next_header;
	header->size = header->size + next_header->size;
	header->next_header = next_header->next_header;
	if(next_header == last_header) {
		last_header = header;
	} else {
		next_header->next_header->prev_header = header;
	}
	return header;
}

/*
	Parameters: pointer to header part of sibling.
	
	delete the sibling from AVL tree if sibling is being coalesced means
	if sibling is either previous block or next block of newly created free block
	or currently freed block.

	returns: 
*/
void coalesce_sibling(struct header *header) {
	
	if(header->left_sibling == NULL) {
		struct header *first_sibling = header;
		struct header *second_sibling = header->right_sibling;
		second_sibling->left_sibling = NULL;
		second_sibling->height = first_sibling->height;
		second_sibling->parent = first_sibling->parent;
		if(first_sibling->parent != NULL) {
			if(first_sibling == first_sibling->parent->left) {
				first_sibling->parent->left = second_sibling;
			} else {
				first_sibling->parent->right = second_sibling;
			}
		} else {
			root = second_sibling;
		}
		second_sibling->left = first_sibling->left;
		if(first_sibling->left != NULL) {
			first_sibling->left->parent = second_sibling;
		}
		second_sibling->right = first_sibling->right;
		if(first_sibling->right != NULL) {
			first_sibling->right->parent = second_sibling;
		}
	} else {
		header->left_sibling->right_sibling = header->right_sibling;
		if(header->right_sibling != NULL) {
			header->right_sibling->left_sibling = header->left_sibling;
		}
	}
	return;
}

/*
	Parameters: pointer to header part of free block

	coalesces the block with next block if next block is free
	coalesces the block with previous block if previous block is free
	delete the free blocks from AVL tree before coalescing. 
	and insert the coalesced block after coalescing.
	
	returns: pointer to same block header if previous block is not free.
			and pointer to previous block otherwise.
*/
void coalesce(struct header *header) {

	if(header->next_header != NULL && header->next_header->status == 0) {
		if(header->next_header->right_sibling != NULL || header->next_header->left_sibling != NULL) {
			coalesce_sibling(header->next_header);
		} else {
			avl_delete(header->next_header);
		}		
		coalesce_next(header);
	}
	if(header->prev_header != NULL && header->prev_header->status == 0) {
		if(header->prev_header->right_sibling != NULL || header->prev_header->left_sibling != NULL) {
			coalesce_sibling(header->prev_header);
		} else {
			avl_delete(header->prev_header);
		}
		header = header->prev_header;
		coalesce_next(header);
	}

	reset_header(header);
	header->data_size = 0;
	avl_insert(header);
	return;
}

/*
	Parameters: pointer to header parts of block next to which new free block is 
				created, size of next new block
	
	returns: 
*/
void create_next_block(struct header *header, size_t next_block_size) {

	struct header *next_block_header = (struct header *)((char *)header + header->size);
	create_header(next_block_header, next_block_size, 0, 0, header, header->next_header);
	if(next_block_header->next_header != NULL) {
		next_block_header->next_header->prev_header = next_block_header;
	} else {
		last_header = next_block_header;
	}
	header->next_header = next_block_header;

	coalesce(next_block_header);
	return;
}

/*
	Parameters: pointer to header of free block, new size of block.

	mm_malloc calls this method to allocate the size when best fit free
	block is found with requested size of block by user.
	creates the free block next to same block if requested size of block
	is less than size of block and free block has size atlest equal to 
	HEADER_SIZE, otherwise don't create the free block.
	
	returns:
*/
void allocate_block(struct header *header, size_t new_block_size) {

	size_t old_block_size = header->size;
	size_t next_block_size = old_block_size - new_block_size;
	if(next_block_size < HEADER_SIZE) {

		avl_delete(header);
		reset_header(header);
		header->status = 1;
		header->data_size = new_block_size - HEADER_SIZE;

		return;
	}
	avl_delete(header);
	reset_header(header);

	init_header(header, new_block_size, new_block_size - HEADER_SIZE, 1);
	create_next_block(header, next_block_size);

	return;
}

/*
	Parameters: pointer to header part of block, new size of block

	mm_realloc calls this method before searching for best fit free block
	if it is possible to resize the block by shrinking or by extending(if next
	block is free) the resize the block in place and creates the free block
	next to same block if size is left(at least equals to HEADER_SIZE) 
	after resizing.
	
	returns: pointer to header part of resized block.
*/
struct header *realloc_in_place(struct header *header, size_t new_block_size) {

	size_t old_block_size = header->size;
	if(old_block_size == new_block_size) {
		header->data_size = new_block_size - HEADER_SIZE;
		return header;
	}
	// shrink inplace
	if(old_block_size > new_block_size) { 
		size_t next_block_size = old_block_size - new_block_size;

		if(next_block_size < HEADER_SIZE) {
			header->data_size = new_block_size - HEADER_SIZE;
			return header; // do not reallocate
		}
		header->size = new_block_size;
		header->data_size = new_block_size - HEADER_SIZE;
		create_next_block(header, next_block_size);

		return header;
	}
	// extend in place
	struct header *next_block = header->next_header;
	if(next_block != NULL && next_block->status == 0 && new_block_size <= old_block_size + next_block->size) {

		size_t next_block_size = next_block->size;
		size_t new_next_block_size = header->size + next_block_size - new_block_size;
		struct header *next_next_block = next_block->next_header;
		avl_delete(next_block);
		if(new_next_block_size < HEADER_SIZE) {
			header->size = old_block_size + next_block_size;
			header->data_size = new_block_size - HEADER_SIZE;
			header->next_header = next_next_block;
			if(next_block == last_header) {
				last_header = header;
			} else {
				next_next_block->prev_header = header;
			}
			return header;
		}
		header->size = new_block_size;
		header->data_size = new_block_size - HEADER_SIZE;
		header->next_header = next_next_block;
		create_next_block(header, new_next_block_size);
		return header;
	}

	return NULL;
}

/*
	Parameters: pointer to header part of target block, pointer to header
				part of source block

	copies the data of source block to target block.
	
	returns: pointer to header part of target block.
*/
struct header *copy_body(struct header *to, struct header *from) {
	// use data_size to copy not total size
	size_t body_size = from->data_size;
	memcpy(get_body_pointer(to), get_body_pointer(from), body_size);
	return to;
}








//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++   Managing AVL Tree of free blocks   ++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*
	Parameters: shrot in a, and b
	
	returns: maximum of a and b
*/
short int max(short int a, short int b) {

	return a > b ? a : b;
}

/*
	Parameters: integer a
	
	returns: absolute value of a
*/
int abs(int a) {

	return a > 0 ? a : -a;
}

/*
	Parameters: node u and v
	
	make the parent of u, to parent of v and
	child of parent to v.

	returns: 
*/
void transplant(struct header *u, struct header *v) {

	if(u == root) {
		root = v;
	} else if(u == u->parent->left) {
		u->parent->left = v;
	} else {
		u->parent->right = v;
	}
	if(v != NULL) {
		v->parent = u->parent;
	}
}

/*
	Parameters: node y

	perform left rotation on node y
	
	returns: 
*/
void left_rotate(struct header *y) {

	struct header *x = y->right;
	y->right = x->left;
	if(y->right != NULL) {
		y->right->parent = y;
	}
	transplant(y, x);
	x->left = y;
	x->left->parent = x;
}

/*
	Parameters: node y

	perform right rotation on node y
	
	returns: 
*/
void right_rotate(struct header *y) {

	struct header *x = y->left;
	y->left = x->right;
	if(y->left != NULL) {
		y->left->parent = y;
	}
	transplant(y, x);
	x->right = y;
	x->right->parent = x;
}

/*
	Parameters: node

	perform left rotation on node.
	
	returns: 
*/
void LL_rotation(struct header *node) {

	left_rotate(node);
}

/*
	Parameters: node

	perform right rotation on node.
	
	returns: 
*/
void RR_rotation(struct header *node) {

	right_rotate(node);
}

/*
	Parameters: node

	perform left-right rotation on node.
	
	returns: 
*/
void LR_rotation(struct header *node) {

	left_rotate(node->left);
	right_rotate(node);
}

/*
	Parameters: node

	perform right-left rotation on node.
	
	returns: 
*/
void RL_rotation(struct header *node) {

	right_rotate(node->right);
	left_rotate(node);
}

/*
	Parameters: node
	
	returns: height of node in AVL tree.
*/
int height(struct header *node) {

	short int l = -1, r = -1;
	if(node->left != NULL) l = node->left->height;
	if(node->right != NULL) r = node->right->height;
	return max(l, r) + 1;
}

/*
	Parameters: node
	
	returns: balance factor of node in AVL tree
*/
int balance_factor(struct header *node) {

	short int l = -1, r = -1;
	if(node->left != NULL) l = height(node->left);
	if(node->right != NULL) r = height(node->right);
	return l - r;
}

/*
	Parameters: node

	rebalance the node by performing rotations.
	
	returns: new node which replaces the node passed in parameters
*/
struct header *rebalance(struct header *node) {

	int bal_factor = balance_factor(node);
	if(bal_factor == 2) {
		if(balance_factor(node->left) == -1) {
			LR_rotation(node);
		} else {
			RR_rotation(node);
		}
		node = node->parent;
		node->left->height = height(node->left);
		node->right->height = height(node->right);

	} else if(bal_factor == -2) {
		if(balance_factor(node->right) == 1) {
			RL_rotation(node);
		} else {
			LL_rotation(node);
		}
		node = node->parent;
		node->left->height = height(node->left);
		node->right->height = height(node->right);
	}
	node->height = height(node);
	return node;
}

/*
	Parameters: root of AVL tree, size to search.

	search best fit block in AVL tree.
	
	returns: pointer to best fit block NULL if 
			no best fit block is found.
*/
struct header *search_best(struct header *root, size_t size) {

	if(root == NULL || root->size == size) return root;
	if(root->size > size) {
		struct header *best = search_best(root->left, size);
		return best == NULL? root: best;
	} else {
		struct header *best = search_best(root->right, size);
		return best;
	}
}

/*
	Parameters: size to search for best fit.

	calls search_best method.
	
	returns: pointer to best fit block if block has no sibling. 
			pointer to second sibling otherwise.
			NULL if no best fit block in AVL tree.
*/
struct header *avl_get_best_fit(size_t size) {

	struct header *node = search_best(root, size);
	if(node == NULL || node->right_sibling == NULL) return node;
	return node->right_sibling;
}

/*
	Parameters: pointer to first sibling, pointer to new block with same 
				size as siblings.

	insert the new free block as second sibling.
	
	returns: 
*/
void insert_sibling(struct header *node, struct header *new_node) {

	new_node->right_sibling = node->right_sibling;
	if(node->right_sibling != NULL) node->right_sibling->left_sibling = new_node;
	node->right_sibling = new_node;
	new_node->left_sibling = node;
}

/*
	Parameters: pointer to header of free block.

	insert the free block into AVL tree

	returns: 
*/
void avl_insert(struct header *new_node) {

	struct header *parent = NULL;
	struct header *node = root;
	while(node != NULL) {
		parent = node;
		if(node->size > new_node->size) {
			node = node->left;
		} else if(node->size < new_node->size) {
			node = node->right;
		} else {
			insert_sibling(node, new_node);
			return;
		}
	}

	if(parent == NULL) {
		root = new_node;
	} else if(parent->size > new_node->size) {
		parent->left = new_node;
		new_node->parent = parent;
	} else {
		parent->right = new_node;
		new_node->parent = parent;
	}
	node = new_node;
	int bal_factor;
	while(node != NULL) {
		bal_factor = abs(balance_factor(node));
		if(bal_factor == 2) {
			rebalance(node);
			break;
		}
		node->height = height(node);
		node = node->parent;
	}
}

/*
	Parameters: node
	
	returns: block with minimum size in subtree rooted by node.
*/
struct header *get_min(struct header *node) {

	if(node == NULL) return NULL;
	while(node->left != NULL) {
		node = node->left;
	}
	return node;
}

/*
	Parameters: pointer to second sibling.

	delete the second sibling from doubly linked list of siblings.
	
	returns: 
*/
void delete_second_sibling(struct header *node) {

	if(node->right_sibling != NULL) {
		node->right_sibling->left_sibling = node->left_sibling;
	}
	node->left_sibling->right_sibling = node->right_sibling;
}

/*
	Parameters: pointer to header of free block.

	delete the free block from AVL tree
	
	returns: 
*/
void avl_delete(struct header *node) {
	// node is second sibling delete node. we only return pointer to second sibling
	// if there are more than one sibling during best fit search.
	if(node->left_sibling != NULL && node->left_sibling->left_sibling == NULL) { 
		delete_second_sibling(node);
		return;
	}
	// node has no siblings it is first sibling.
	struct header *parent;
	struct header *x;
	struct header *y;
	struct header *temp;

	parent = node->parent;
	if(node->left == NULL) {
		transplant(node, node->right);
	} else if(node->right == NULL) {
		transplant(node, node->left);
	} else {
		y = get_min(node->right);
		x = y->right;
		if(y == node->right) {
			parent = y;
		} else {
			parent = y->parent;
			transplant(y, x);
			y->right = node->right;
			y->right->parent = y;
		}
		transplant(node, y);
		y->left = node->left;
		y->left->parent = y;
	}
	temp = parent;
	while(temp != NULL) {
		temp = rebalance(temp);
		temp = temp->parent;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    It was My Lib    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



void *init_mem_sbrk_break = NULL;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {

	root = NULL;
	first_header = NULL;
	last_header = NULL;
    return 0;		//Returns 0 on successfull initialization.
}

/* 
	mm_malloc - uses find_best_fit_blk to get a free block of size given
	as parameter. The address of the data part of this block is returned. 
*/
void *mm_malloc(size_t size) {	

	if(size <= 0){		// Invalid request size
		return NULL;
	}
	struct header *header;
	size_t block_size = size + HEADER_SIZE; // total size
	block_size = ALIGN(block_size);

	if(first_header != NULL) {	// find best block
		header = avl_get_best_fit(block_size);
		if(header != NULL) {
			allocate_block(header, block_size);
			return (void *)get_body_pointer(header);
		}
	}
	
	header = (struct header *)mem_sbrk(block_size);
	create_header(header, block_size, block_size - HEADER_SIZE, 1, last_header, NULL);
	if(first_header == NULL) {
		first_header = header;
	} else {
		last_header->next_header = header;
	}

	last_header = header;
	return (void *)get_body_pointer(header);
}

/*
	mm_free - inserts the given block into the free list.
	It also uses coalesce to merge the previous and next blocks
	if they are also free.
*/
void mm_free(void *ptr) {

	struct header *header = get_header_pointer((char *)ptr);
	header->status = 0;
	coalesce(header);
	return;
}

/*
	mm_realloc - reallocates a given block pointer to a new location if the size given in parameters
	is greater than the size of given block pointer by calling mm_malloc and mm_copy. If the given
	size is less than or equal to the size of the block pointer then the same block pointer is 
	returned after calling the shrink_blk.    
*/
void *mm_realloc(void *ptr, size_t size) {	
	if(ptr == NULL){			//memory was not previously allocated
		return mm_malloc(size);
	}
	if(size == 0){				//new size is zero
		mm_free(ptr);
		return NULL;
	}

	size_t block_size = size + HEADER_SIZE;
	block_size = ALIGN(block_size);

	struct header *old_header = get_header_pointer((char *)ptr);
	struct header *new_header = realloc_in_place(old_header, block_size);
	if(new_header != NULL) {
		return (void *)get_body_pointer(new_header);
	}
	if(old_header == last_header) { // if it is last and no best node so extend
		struct header *avl_best = avl_get_best_fit(block_size);
		if(avl_best == NULL) {
			size_t extra_size = HEADER_SIZE + size - old_header->size;
			extra_size = ALIGN(extra_size);
			old_header->size += extra_size;
			old_header->data_size = old_header->size - HEADER_SIZE;
			mem_sbrk(extra_size);
			return (void *)get_body_pointer(old_header);
		}
	}
	char *new_body_pointer = (char *)mm_malloc(size);
	new_header = get_header_pointer(new_body_pointer);

	copy_body(new_header, old_header);
	mm_free((void *)get_body_pointer(old_header));
	return (void *)get_body_pointer(new_header);
}














