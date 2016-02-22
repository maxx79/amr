#include "buffer_manager.h"
#include "list/list.h"
#include <stdlib.h>
#include <stdio.h>

BufferDescriptor *bm_create(int buffer_frame_size) {
    BufferDescriptor *buffer_descriptor;
    if (buffer_frame_size <=0)  {
	return 0;
    }
    buffer_descriptor  = (BufferDescriptor*)malloc(sizeof(BufferDescriptor));
    buffer_descriptor->frame_size = buffer_frame_size;
    buffer_descriptor->head_node = buffer_descriptor->tail_node = list_create();
    list_put_data(buffer_descriptor->head_node, malloc(buffer_descriptor->frame_size));
    buffer_descriptor->position = 0;
    return buffer_descriptor;
}
void bm_release(BufferDescriptor *buffer_descriptor) {
    ListNode *current_list_node;
    void *data;
    if (buffer_descriptor == 0) {
	return;
    }
    current_list_node = buffer_descriptor->head_node;
    while(1) {
      data = list_get_data(current_list_node);
      if (data != 0) {
        free(data);
      }
      if (!list_is_tail(current_list_node)) {
         current_list_node = list_next(current_list_node);
      } else {
	    break;
	}
    }
    list_release(buffer_descriptor->head_node);
    buffer_descriptor->head_node = 0;
    buffer_descriptor->tail_node = 0;
    buffer_descriptor->position = 0;
    free(buffer_descriptor);
}
int bm_get_length(BufferDescriptor *buffer_descriptor) {
    int length = 0;
    ListNode *current_list_node;
    if (buffer_descriptor == 0) {
	return 0;
    }
    current_list_node = buffer_descriptor->head_node;
    while(!list_is_tail(current_list_node)) {
	length += buffer_descriptor->frame_size;
	current_list_node = list_next(current_list_node);
    }
    length += buffer_descriptor->position;
    return length;
}
void bm_to_array(BufferDescriptor *buffer_descriptor, void *data) {
    ListNode *current_list_node;
    int offset = 0;
    void *node_data;
    if (data == 0 || buffer_descriptor == 0) {
      return;
    }
    current_list_node = buffer_descriptor->head_node;
    while(!list_is_tail(current_list_node)) {
        node_data = list_get_data(current_list_node);
        if (node_data != 0)
        {
	    memcpy(((unsigned char *)data + offset), node_data, buffer_descriptor->frame_size);
	    offset += buffer_descriptor->frame_size;
	}
	current_list_node = list_next(current_list_node);
    }        
    if (buffer_descriptor->position != 0) {
	node_data = list_get_data(current_list_node);
	if (node_data != 0) {
	    memcpy(((unsigned char *)data + offset), node_data, buffer_descriptor->position);
	}
    }
}
void * bm_get_as_array(BufferDescriptor *buffer_descriptor) {
    int length;
    void *data;
    if (buffer_descriptor == 0) {
      return 0;
    }
    length = bm_get_length(buffer_descriptor);
    if (length > 0) {
	data = malloc(length);
	bm_to_array(buffer_descriptor, data);
    }
    return data;
}
void bm_write_byte(BufferDescriptor *buffer_descriptor, unsigned char b) {
    ListNode *current_list_node;
    void *data;
    if (buffer_descriptor == 0) {
	return;
    }
    current_list_node = buffer_descriptor->tail_node;
    if (buffer_descriptor->position >= buffer_descriptor->frame_size) {
	current_list_node = buffer_descriptor->tail_node = list_add(current_list_node);
	data = malloc(buffer_descriptor->frame_size);
	list_put_data(current_list_node, data);
	buffer_descriptor->position = 0;
    }
     else {
        data = list_get_data(current_list_node);
    }
    *(((unsigned char *)data) + buffer_descriptor->position) = b;
    buffer_descriptor->position++;
}
void bm_write_bytes(BufferDescriptor *buffer_descriptor, unsigned char *src, int length) {
	ListNode *current_list_node;
	void *dst_data;
	int remain;
	int available;
	if (buffer_descriptor == 0 || src == 0 || length <= 0) {
	    return;
	}
	current_list_node = buffer_descriptor->tail_node;
	remain = length;
	while(remain > 0) {
	   available = buffer_descriptor->frame_size - buffer_descriptor->position;
	   dst_data = list_get_data(current_list_node);
	   if (available >= remain) {
		memcpy((unsigned char *)dst_data + buffer_descriptor->position, src, remain);
		buffer_descriptor->position += remain;
		src += remain;
		remain = 0;
	   } 
	   else {
		memcpy((unsigned char *)dst_data + buffer_descriptor->position, src, available);
		remain -= available;
		src += available;
		current_list_node = buffer_descriptor->tail_node = list_add(current_list_node);	
		list_put_data(current_list_node, malloc(buffer_descriptor->frame_size));
		buffer_descriptor->position = 0;
	   }
	}
}
void bm_write_short(BufferDescriptor *buffer_descriptor, short value) {
    bm_write_bytes(buffer_descriptor, (void *)&value, sizeof(short));
}
void bm_write_int(BufferDescriptor *buffer_descriptor, int value) {
    bm_write_bytes(buffer_descriptor, (void *)&value, sizeof(int));
}
void bm_write_long(BufferDescriptor *buffer_descriptor, long value) {
    bm_write_bytes(buffer_descriptor, (void *)&value, sizeof(long));
}
void bm_write_float(BufferDescriptor *buffer_descriptor, float value) {
    bm_write_bytes(buffer_descriptor, (void *)&value, sizeof(float));
}
void bm_write_double(BufferDescriptor *buffer_descriptor, double value) {
    bm_write_bytes(buffer_descriptor, (void *)&value, sizeof(double));
}


