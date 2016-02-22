#include <stdlib.h>
#include "list.h"
ListNode *list_create() {
    ListNode *first_list_node;
    first_list_node = (ListNode*)malloc(sizeof(ListNode));
    first_list_node->prev = 0;
    first_list_node->next = 0;
    first_list_node->data = 0;
    return first_list_node;
}
void list_put_data(ListNode *list_node, void *data) {
    if (list_node == 0) {
	return;
    }
    list_node->data = data;
} 
void *list_get_data(ListNode *list_node) {
    if (list_node == 0) {
	return 0;
    }
    return list_node->data;
}
int list_is_head(ListNode *list_node) {
    if (list_node == 0) {
	return 0;
    }
    return list_node->prev == 0;
} 
int list_is_tail(ListNode *list_node) {
    if (list_node == 0) {
	return 0;
    }
    return list_node->next == 0;
}
ListNode * list_get_head(ListNode *list_node) {
    if (list_node == 0) {
	return 0;
    }
    while(list_node->prev != 0) {
	list_node = list_node->prev;
    }
    return list_node;
}
ListNode * list_get_tail(ListNode *list_node) {
    if (list_node == 0) {
	return 0;
    }
    while(list_node->next != 0) {
	list_node = list_node->next;
    }
    return list_node;
}
ListNode * list_prev(ListNode *list_node) {
    if (list_node == 0) {
	return 0;
    }
    return list_node->prev;
}
ListNode * list_next(ListNode *list_node) {
    if (list_node == 0) {
	return 0;
    }
    return list_node->next;
}
ListNode *list_add_node(ListNode *list_node, ListNode *user_list_node) {
    ListNode *new_list_node, *last_list_node;
    if (list_node == 0 || user_list_node == 0) {
	return 0;
    }
    last_list_node = list_get_tail(list_node);
    last_list_node->next = user_list_node;
    user_list_node->prev = last_list_node;
    user_list_node->next = 0;
    return user_list_node;
}
ListNode *list_add(ListNode *list_node) {    
    ListNode *new_list_node;
    if (list_node == 0) {
	return 0;
    }
    new_list_node = (ListNode *)malloc(sizeof(ListNode));
    new_list_node->data = 0;
    return list_add_node(list_node, new_list_node);
}
ListNode *list_remove(ListNode *list_node) {
    ListNode *prev_list_node, *next_list_node;
    if (list_node == 0) {
	return 0;
    }
    prev_list_node = list_node->prev;
    next_list_node = list_node->next;
    if (prev_list_node != 0) {
	prev_list_node->next = next_list_node;
    }
    if (next_list_node != 0) {
	next_list_node->prev = prev_list_node;
    }
    free(list_node);
    return prev_list_node != 0 ? prev_list_node : next_list_node;
}
int list_get_count(ListNode *list_node) {
    int count = 0;
    ListNode *prev_list_node, *next_list_node;
    if (list_node == 0) {
	return 0;
    }
    count++;
    prev_list_node = list_node->prev;
    next_list_node = list_node->next;
    while(prev_list_node != 0 || next_list_node != 0) {
	if (prev_list_node != 0) {
	    count++;
	    prev_list_node = prev_list_node->prev;
	}
	if (next_list_node != 0) {
	    count++;
	    next_list_node = next_list_node->next;
	}
    }
    return count;
}
int list_contains(ListNode *list_node, void *pattern, int pattern_size) {
    ListNode *prev_list_node;
    ListNode *next_list_node;
    if (list_node == 0 || pattern == 0 || pattern_size <= 0) {
	return 0;
    }
    if (!memcmp(list_node->data, pattern, pattern_size)) {
	return 1;
    }
    prev_list_node = list_node->prev;
    next_list_node = list_node->next;
    while(prev_list_node !=0 || next_list_node !=0) {
	if (prev_list_node != 0) {
	    if (!memcmp(prev_list_node->data, pattern, pattern_size)) {
		return 1;
	    }
	    prev_list_node = prev_list_node->prev;
	}
	if (next_list_node != 0) {
	    if (!memcmp(next_list_node->data, pattern, pattern_size)) {
		return 1;
	    }
	    next_list_node = next_list_node->next;
	}
    }
    return 0;
}
ListNode *list_find(ListNode *list_node, void *pattern, int pattern_size) {
    ListNode *prev_list_node;
    ListNode *next_list_node;
    if (list_node == 0 || pattern == 0 || pattern_size <= 0) {
	return 0;
    }
    if (!memcmp(list_node->data, pattern, pattern_size)) {
	return list_node;
    }
    prev_list_node = list_node->prev;
    next_list_node = list_node->next;
    while(prev_list_node != 0 || next_list_node != 0) {
	if (prev_list_node != 0) {
	    if (!memcmp(prev_list_node->data, pattern, pattern_size)) {
		return prev_list_node;
	    }
	    prev_list_node = prev_list_node->prev;
	}
	if (next_list_node != 0) {
	    if (!memcmp(next_list_node->data, pattern, pattern_size)) {
		return next_list_node;
	    }
	    next_list_node = next_list_node->next;
	}
    }
    return 0;
}

void list_release(ListNode *list_node) {
    ListNode *prev_list_node, *next_list_node;
    if (list_node == 0) {
	return;
    }
    prev_list_node = list_node->prev;
    next_list_node = list_node->next;
    while(prev_list_node != 0 || next_list_node != 0) {
	if (prev_list_node != 0) {
	    list_node = prev_list_node->prev;
	    free(prev_list_node);
	    prev_list_node = list_node;
	}
	if (next_list_node != 0) {
	    list_node = next_list_node->next;
	    free(next_list_node);
	    next_list_node = list_node;
	}
    }
}
