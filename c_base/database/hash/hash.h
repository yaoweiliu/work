#ifndef __HASH_H__
#define __HASH_H__

#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) *__mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member));})

struct hash_node
{
	struct hash_node *next;
	struct hash_node **pprev;
};

struct hash_head
{
	struct hash_node *first;
};

#define INIT_HLIST_HEAD(ptr)	((ptr)->first = NULL)

void init_hlist_node(struct hash_node *n);
void hlist_delete(struct hash_node *n);
void hlist_delete_init(struct hash_node *n);
void hlist_add_head(struct hash_head *h, struct hash_node *n);
void hlist_add_before(struct hash_node *n, struct hash_node *next);
void hlist_add_behind(struct hash_node *n, struct hash_node *prev);

#endif

