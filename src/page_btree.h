#ifndef _RL_PAGE_BTREE_H
#define _RL_PAGE_BTREE_H

#include "rlite.h"

#define rl_btree_nocache_destroy(db, btree) (db->driver_type != RL_MEMORY_DRIVER ? rl_btree_destroy(db, btree) : RL_OK)
#define rl_btree_node_nocache_destroy(db, node) (db->driver_type != RL_MEMORY_DRIVER ? rl_btree_node_destroy(db, node) : RL_OK);

struct rl_data_type;
struct rl_btree;
struct rl_btree_node;

typedef struct {
	struct rl_data_type *btree_type;
	struct rl_data_type *btree_node_type;
	int score_size;
	int value_size;
	int (*cmp)(void *v1, void *v2);
#ifdef DEBUG
	int (*formatter)(void *v, char **str, int *size);
#endif
} rl_btree_type;

int sha1_cmp(void *v1, void *v2);
#ifdef DEBUG
int sha1_formatter(void *v, char **str, int *size);
#endif

typedef struct rl_key {
	unsigned char type;
	long string_page;
	long value_page;
} rl_key;

extern rl_btree_type rl_btree_type_set_long;
extern rl_btree_type rl_btree_type_hash_long_long;
extern rl_btree_type rl_btree_type_hash_sha1_long;
extern rl_btree_type rl_btree_type_hash_sha1_key;
extern rl_btree_type rl_btree_type_hash_sha1_double;

typedef struct rl_btree_node {
	void **scores;
	// children is null when the node is a leaf
	// when created, allocs size+1.
	long *children;
	void **values;
	// size is the number of children used; allocs the maximum on creation
	long size;
} rl_btree_node;

typedef struct rl_btree {
	struct rlite *db;
	long max_node_size; // maximum number of scores in a node
	long height;
	rl_btree_type *type;
	long root;
	long number_of_elements;
} rl_btree;

typedef struct {
	struct rlite *db;
	rl_btree *btree;
	long size;
	struct rl_btree_iterator_nodes {
		long position;
		rl_btree_node *node;
	} nodes[];
} rl_btree_iterator;

void rl_btree_init();
int rl_btree_create(struct rlite *db, rl_btree **btree, rl_btree_type *type, long max_node_size);
int rl_btree_destroy(struct rlite *db, void *btree);
int rl_btree_node_destroy(struct rlite *db, void *node);
int rl_btree_add_element(struct rlite *db, rl_btree *btree, void *score, void *value);
int rl_btree_remove_element(struct rlite *db, rl_btree *btree, void *score);
int rl_btree_find_score(struct rlite *db, rl_btree *btree, void *score, void **value, rl_btree_node *** nodes, long **positions);
int rl_print_btree(struct rlite *db, rl_btree *btree);
int rl_btree_is_balanced(struct rlite *db, rl_btree *btree);
int rl_flatten_btree(struct rlite *db, rl_btree *btree, void *** scores, long *size);

int rl_btree_iterator_create(struct rlite *db, rl_btree *btree, rl_btree_iterator **iterator);
int rl_btree_iterator_next(rl_btree_iterator *iterator, void **score, void **value);
int rl_btree_iterator_destroy(rl_btree_iterator *iterator);

int rl_btree_serialize(struct rlite *db, void *obj, unsigned char *data);
int rl_btree_deserialize(struct rlite *db, void **obj, void *context, unsigned char *data);

int rl_btree_node_serialize_hash_sha1_long(struct rlite *db, void *obj, unsigned char *data);
int rl_btree_node_deserialize_hash_sha1_long(struct rlite *db, void **obj, void *context, unsigned char *data);

int rl_btree_node_serialize_hash_sha1_key(struct rlite *db, void *obj, unsigned char *data);
int rl_btree_node_deserialize_hash_sha1_key(struct rlite *db, void **obj, void *context, unsigned char *data);

int rl_btree_node_serialize_set_long(struct rlite *db, void *obj, unsigned char *data);
int rl_btree_node_deserialize_set_long(struct rlite *db, void **obj, void *context, unsigned char *data);

int rl_btree_node_serialize_hash_long_long(struct rlite *db, void *obj, unsigned char *data);
int rl_btree_node_deserialize_hash_long_long(struct rlite *db, void **obj, void *context, unsigned char *data);

int rl_btree_node_serialize_hash_sha1_double(struct rlite *db, void *obj, unsigned char *data);
int rl_btree_node_deserialize_hash_sha1_double(struct rlite *db, void **obj, void *context, unsigned char *data);

#endif
