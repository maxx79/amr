#ifndef __BUFFER_MANAGER__
#define __BUFFER_MANAGER__
#include "list/list.h"
#define BUFFER_FRAME_SIZE 1024
struct TagBufferDescriptor {
    ListNode *head_node;
    ListNode *tail_node;
    int position;
    int frame_size;
};
typedef struct TagBufferDescriptor BufferDescriptor;
BufferDescriptor *bm_create(int);
void bm_release(BufferDescriptor *);
int bm_get_length(BufferDescriptor *);
void bm_to_array(BufferDescriptor *, void *);
void *bm_get_as_array(BufferDescriptor *);
void bm_write_byte(BufferDescriptor *, unsigned char);
void bm_write_bytes(BufferDescriptor *, unsigned char *, int);
void bm_write_short(BufferDescriptor *, short);
void bm_write_short(BufferDescriptor *, short);
void bm_write_int(BufferDescriptor *, int);
void bm_write_long(BufferDescriptor *, long);
#endif