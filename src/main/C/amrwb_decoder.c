#include <stdio.h>
#include <string.h>
#include <dec_if.h>
#include "buffer_manager.h"
#include "amr.h"
const int amrwb_sizes[] = { 17, 23, 32, 36, 40, 46, 50, 58, 60, 5, -1, -1, -1, -1, -1, 0 };
int amrwb_decode(BufferDescriptor *bm_handler, void *input_data, long length) {
    const char *amr_header = "#!AMR-WB\n";
    register long offset = 0;
    void *amr_handler;
    register int n, size, i;
    short temp_buffer[AMR_DECODED_FRAME_SIZE];
    unsigned char littleendian[AMR_DECODED_FRAME_SIZE * 2];
    unsigned char *ptr;

    if (bm_handler == 0 || input_data == 0 || length <= 9L) {
	return INVALID_ARGUMENT;
    }
    if (memcmp(input_data, amr_header, 9)) {
	return INVALID_FORMAT;
    }
    offset += 9L;
    length -= 9L;
    amr_handler = D_IF_init();
    if (amr_handler == 0) {
	return INITIALIZATION_ERROR;
    }
    while(length > 0) {
	n = ((unsigned char *)input_data)[offset];/*frame size*/
	size = amrwb_sizes[(n >> 3) & 0x0f];
	if (size <= 0) {
	    break;
	}
	if (length < size) {
	    break;
	}
	D_IF_decode(amr_handler, ((unsigned char*)input_data) + offset, temp_buffer, 0);
	offset += (long)(size + 1);
	length -= (long)(size + 1);
	ptr = littleendian;
	for (i = 0; i < AMR_DECODED_FRAME_SIZE; i++) {
	    *ptr++ = (temp_buffer[i] >> 0) & 0xff;
	    *ptr++ = (temp_buffer[i] >> 8) & 0xff;
	}
	bm_write_bytes(bm_handler, littleendian, AMR_DECODED_FRAME_SIZE);
    }
    Decoder_Interface_exit(amr_handler);
    return OK;
}
void *amrwb_decode_frame(void *amr_handler, void *input_data, void *output_data) {
	unsigned char *ptr;
	short temp_buffer[AMR_DECODED_FRAME_SIZE];
	register int size, n, i;
	if (amr_handler == 0 || input_data == 0 || output_data == 0) {
	    return 0;
	}
	n = ((unsigned char *)input_data)[0];/*frame size*/
	size = amrwb_sizes[(n >> 3) & 0x0f];
	if (size <= 0) {
	    return 0;
	}
	D_IF_decode(amr_handler, (unsigned char*)input_data, temp_buffer, 0);
	ptr = (unsigned char*)output_data;
	for (i = 0; i < AMR_DECODED_FRAME_SIZE; i++) {
	    *ptr++ = (temp_buffer[i] >> 0) & 0xff;
	    *ptr++ = (temp_buffer[i] >> 8) & 0xff;
	}
	return output_data;
}

