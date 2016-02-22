#include <stdio.h>
#include <string.h>
#include <interf_enc.h>
#include "buffer_manager.h"
#include "amr.h"
int amrnb_encode(BufferDescriptor *bm_handler, int mode, int dtx, void *input_data_ptr, long input_data_len) {
    const char *amr_header = "#!AMR\n";
    void *h;
    int i;
    short bigendian[AMR_DECODED_FRAME_SIZE/2];
    unsigned char output_buffer[500];
    unsigned char *byte_data_ptr;
    int output_buffer_len;
    int offset;
    if (bm_handler == 0 || mode < 0 || dtx < 0 || input_data_ptr == 0 || input_data_len <= 0L) {
	return INVALID_ARGUMENT;
    }
    h = Encoder_Interface_init(dtx);
    if (h == 0) {
	return INITIALIZATION_ERROR;
    }
    bm_write_bytes(bm_handler, amr_header, 6L);
    offset = 0;
    while(input_data_len >= AMR_DECODED_FRAME_SIZE) {	
	for (i = 0; i < AMR_DECODED_FRAME_SIZE/2; i++) {
		byte_data_ptr = ((unsigned char *)input_data_ptr) + offset;
		bigendian[i] = *(byte_data_ptr) | (*(byte_data_ptr + 1) << 8);
		offset += 2;
	}
	output_buffer_len = (long)Encoder_Interface_Encode(h, mode, (void*)bigendian, (void *)output_buffer, 0);
	if (output_buffer_len <= 0) {
	    break;
	}
	bm_write_bytes(bm_handler, (void*)output_buffer, output_buffer_len);
	input_data_len -= AMR_DECODED_FRAME_SIZE;
    }
    Encoder_Interface_exit(h);
    return OK;
}
void *amrnb_encode_frame(int mode, void *h, void *input_data_ptr, void* output_data, long *output_data_len) {	
	int i;
	unsigned char* byte_data_ptr;
	short bigendian[AMR_DECODED_FRAME_SIZE/2];
	if (mode < 0 || h == 0 || input_data_ptr == 0 || output_data == 0 || output_data_len == 0) {
	    return 0;
	}
	/* convert to big endian */
	for (i = 0; i < AMR_DECODED_FRAME_SIZE/2; i++) {
		byte_data_ptr = ((unsigned char *)input_data_ptr) + (i  << 1);
		bigendian[i] = *byte_data_ptr | (*(byte_data_ptr + 1) << 8); 
	}
	*output_data_len = (long)Encoder_Interface_Encode(h, mode, (void*)bigendian, output_data, 0);
	return output_data;
}
