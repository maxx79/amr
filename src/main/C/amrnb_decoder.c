#include <stdio.h>
#include <string.h>
#include <interf_dec.h>
#include "buffer_manager.h"
#include "amr.h"
const int amrnb_sizes[] = { 12, 13, 15, 17, 19, 20, 26, 31, 5, 6, 5, 5, 0, 0, 0, 0 };
int amrnb_decode(BufferDescriptor *bm_handler, void *input_data, long length) {
    const char *amr_header = "#!AMR\n";
    void *output_data;
    register long offset = 0;
    void *amr_handler;
    register int n, size, i;
    short temp_buffer[AMR_DECODED_FRAME_SIZE/2];
    unsigned char littleendian[AMR_DECODED_FRAME_SIZE];
    unsigned char *ptr;
    if (bm_handler == 0 || input_data == 0 || length <= 6L) {
	return INVALID_ARGUMENT;
    }
    if (memcmp(input_data, amr_header, 6)) {
	return INVALID_FORMAT;
    }
    offset += 6L;
    length -= 6L;
    amr_handler = Decoder_Interface_init();
    if (amr_handler == 0) {
	return INITIALIZATION_ERROR;
    }
    while(length > 0) {
	n = ((unsigned char *)input_data)[offset];/*frame size*/
	size = amrnb_sizes[(n >> 3) & 0x0f];
	if (size <= 0) {
	    break;
	}
	if (length < size) {
	    break;
	}
	Decoder_Interface_Decode(amr_handler, ((unsigned char*)input_data) + offset, temp_buffer, 0);
	offset += (long)(size+1);
	length -= (long)(size+1);
	ptr = littleendian;
	for (i = 0; i < AMR_DECODED_FRAME_SIZE/2; i++) {
	    *ptr++ = (temp_buffer[i] >> 0) & 0xff;
	    *ptr++ = (temp_buffer[i] >> 8) & 0xff;
	}
	bm_write_bytes(bm_handler, littleendian, AMR_DECODED_FRAME_SIZE);
    }
    Decoder_Interface_exit(amr_handler);
    return OK;
}
void *amrnb_decode_frame(void *amr_handler, void *input_data, void *output_data) {
	unsigned char *ptr;
	short temp_buffer[AMR_DECODED_FRAME_SIZE/2];
	register int size, n, i;
	if (amr_handler == 0 || input_data == 0 || output_data == 0) {
	    return 0;
	}
	n = ((unsigned char *)input_data)[0];/*frame size*/
	size = amrnb_sizes[(n >> 3) & 0x0f];
	if (size <= 0) {
	    return 0;
	}
	Decoder_Interface_Decode(amr_handler, (unsigned char*)input_data, temp_buffer, 0);
	ptr = (unsigned char*)output_data;
	for (i = 0; i < AMR_DECODED_FRAME_SIZE/2; i++) {
	    *ptr++ = (temp_buffer[i] >> 0) & 0xff;
	    *ptr++ = (temp_buffer[i] >> 8) & 0xff;
	}
	return output_data;
}

