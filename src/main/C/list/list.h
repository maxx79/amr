#ifndef __LIST__
#define __LIST__
typedef struct TagListNode ListNode;
struct TagListNode {
    ListNode *prev;
    ListNode *next;
    void *data;
};
ListNode *list_create();
void list_put_data(ListNode *, void *);
void *list_get_data(ListNode *);
ListNode *list_get_head(ListNode *);
ListNode *list_next(ListNode *);
ListNode *list_prev(ListNode *);
ListNode *list_get_tail(ListNode *);
ListNode *list_add_node(ListNode *list_node, ListNode *);
ListNode *list_add(ListNode *);
ListNode *list_remove(ListNode *);
int list_get_count(ListNode *);
void list_release(ListNode *);
int list_is_head(ListNode *);
int list_is_tail(ListNode *);
int list_contains(ListNode *, void *, int);
ListNode *list_find(ListNode *, void *, int);
#endif