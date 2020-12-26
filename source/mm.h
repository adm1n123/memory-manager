#include <stdio.h>

extern int mm_init (void);
extern void *mm_malloc (size_t size);
extern void mm_free (void *ptr);
extern void *mm_realloc(void *ptr, size_t size);

// mm2.c

// void create_header(struct header *header, int size, int data_size, short int status, struct header *prev_header, struct header *next_header);
// void init_header(struct header *header, int size, int data_size, short int status);
// void reset_header(struct header *header);
// char *get_body_pointer(struct header *header);
// struct header *get_header_pointer(char *body_ptr);
// void coalesce(struct header *header);
// struct header *coalesce_next(struct header *header);
// void coalesce_sibling(struct header *header);
// void create_next_block(struct header *header, int next_block_size);
// void allocate_block(struct header *header, int new_block_size);
// struct header *realloc_in_place(struct header *header, int new_block_size);
// struct header *copy_body(struct header *to, struct header *from);

// short int max(short int a, short int b);
// int abs(int a);
// void transplant(struct header *u, struct header *v);
// void left_rotate(struct header *y);
// void right_rotate(struct header *y);
// void LL_rotation(struct header *node);
// void RR_rotation(struct header *node);
// void LR_rotation(struct header *node);
// void RL_rotation(struct header *node);
// int height(struct header *node);
// int balance_factor(struct header *node);
// struct header *rebalance(struct header *node);
// struct header *search_best(struct header *root, int size);
// struct header *avl_get_best_fit(int size);
// void insert_sibling(struct header *node, struct header *new_node);
// void avl_insert(struct header *new_node);
// struct header *get_min(struct header *node);
// void delete_second_sibling(struct header *node);
// void avl_delete(struct header *node);


/* 
 * Students work in teams of one or two.  Teams enter their team name, 
 * personal names and login IDs in a struct of this
 * type in their bits.c file.
 */
typedef struct {
    char *teamname; /* ID1+ID2 or ID1 */
    char *name1;    /* full name of first member */
    char *id1;      /* login ID of first member */
    char *name2;    /* full name of second member (if any) */
    char *id2;      /* login ID of second member */
} team_t;

extern team_t team;

