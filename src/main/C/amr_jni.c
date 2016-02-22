#include "amr.h"
#include <interf_enc.h>
#include <interf_dec.h>
#include <dec_if.h>
#include "ru_maximgorin_media_amr_NbDecoder.h"
#include "ru_maximgorin_media_amr_WbDecoder.h"
#include "ru_maximgorin_media_amr_NbEncoder.h"
#include "buffer_manager.h"
/*						AMR NB FUNCTIONS 						*/
JNIEXPORT jlong JNICALL Java_ru_maximgorin_media_amr_NbDecoder_initDecoder
  (JNIEnv *env, jclass class) {
    void *h = Decoder_Interface_init();
    return (jlong)h;

}

/*
 * Class:     ru_maximgorin_media_amr_Decoder
 * Method:    releaseDecoder
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_ru_maximgorin_media_amr_NbDecoder_releaseDecoder
  (JNIEnv *env, jclass class, jlong h) {
    Decoder_Interface_exit((void*)h);
}

/*
 * Class:     ru_maximgorin_media_amr_Decoder
 * Method:    decodeAll
 * Signature: ([B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_NbDecoder_decodeAll
  (JNIEnv *env, jclass class, jbyteArray input_data) {
    long output_data_len;
    jbyte *output_data_ptr;
    jbyte *input_data_ptr;
    jsize input_data_len;
    jbyteArray return_array;
    BufferDescriptor *bm_handler;
    int error_code;

    bm_handler = bm_create(2048);/*buffer size*/
    if (bm_handler == 0) {
	return 0;
    }
    input_data_len = (*env)->GetArrayLength(env, input_data);
    input_data_ptr = (*env)->GetByteArrayElements(env, input_data, 0);
    error_code = amrnb_decode(bm_handler, (void*)input_data_ptr, (long)input_data_len);
    if (error_code != OK) {
	return 0;/*must be thrown an exception*/
    }
    output_data_len = bm_get_length(bm_handler);/*in bytes*/
    if (output_data_len >= 0) {
	return_array = (*env)->NewByteArray(env, (jsize)output_data_len);
	output_data_ptr = (*env)->GetByteArrayElements(env, return_array, 0);
	bm_to_array(bm_handler, output_data_ptr);
	(*env)->SetByteArrayRegion(env, return_array, 0, (jsize)output_data_len, (jbyte*)output_data_ptr);
	(*env)->ReleaseByteArrayElements(env, return_array, output_data_ptr, 0);
    }    
    (*env)->ReleaseByteArrayElements(env, input_data, input_data_ptr, 0);
    bm_release(bm_handler);
    return return_array;
}

/*
 * Class:     ru_maximgorin_media_amr_Decoder
 * Method:    decodeFrame
 * Signature: (J[B[B)V
 */
JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_NbDecoder_decodeFrame
  (JNIEnv *env, jclass class, jlong h, jbyteArray input_data) {
    jbyte *input_data_ptr;
    jbyte output_data[AMR_DECODED_FRAME_SIZE];
    jbyteArray returnArray;
    input_data_ptr = (*env)->GetByteArrayElements(env, input_data, 0);
    if (amrnb_decode_frame((void *)h, (void *)input_data_ptr, (void*)output_data) != 0) {
	returnArray = (*env)->NewByteArray(env, (jsize)AMR_DECODED_FRAME_SIZE);
	(*env)->SetByteArrayRegion(env, returnArray, 0, (jsize)AMR_DECODED_FRAME_SIZE, output_data);
    } 
    (*env)->ReleaseByteArrayElements(env, input_data, input_data_ptr, 0);
    return returnArray;
}
/*						AMR WB FUNCTIONS 						*/
JNIEXPORT jlong JNICALL Java_ru_maximgorin_media_amr_WbDecoder_initDecoder  (JNIEnv *env, jclass class) {
    jlong h = (int)D_IF_init();
    return h;
}

/*
 * Class:     ru_maximgorin_media_amr_WbDecoder
 * Method:    releaseDecoder
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ru_maximgorin_media_amr_WbDecoder_releaseDecoder (JNIEnv *env, jclass class, jlong h) {
    D_IF_exit((void*)h);
}

/*
 * Class:     ru_maximgorin_media_amr_WbDecoder
 * Method:    decodeAll
 * Signature: ([B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_WbDecoder_decodeAll (JNIEnv *env, jclass class, jbyteArray input_data) {
    long output_data_len;
    jbyte *output_data_ptr;
    jbyte *input_data_ptr;
    jsize input_data_len;
    jbyteArray return_array;
    BufferDescriptor *bm_handler;
    int error_code;

    bm_handler = bm_create(2048);/*buffer size*/
    input_data_len = (*env)->GetArrayLength(env, input_data);
    input_data_ptr = (*env)->GetByteArrayElements(env, input_data, 0);
    error_code = amrwb_decode(bm_handler, (void*)input_data_ptr, (long)input_data_len);
    if (error_code != OK) {
	return 0;
    }
    output_data_len = bm_get_length(bm_handler);/*in bytes*/
    if (output_data_len >= 0) {
	return_array = (*env)->NewByteArray(env, (jsize)output_data_len);
	output_data_ptr = (*env)->GetByteArrayElements(env, return_array, 0);
	bm_to_array(bm_handler, output_data_ptr);
	(*env)->SetByteArrayRegion(env, return_array, 0, (jsize)output_data_len, (jbyte*)output_data_ptr);
	(*env)->ReleaseByteArrayElements(env, return_array, output_data_ptr, 0);
    }    
    (*env)->ReleaseByteArrayElements(env, input_data, input_data_ptr, 0);
    bm_release(bm_handler);
    return return_array;
}

/*
 * Class:     ru_maximgorin_media_amr_WbDecoder
 * Method:    decodeFrame
 * Signature: (I[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_WbDecoder_decodeFrame (JNIEnv *env, jclass class, jlong h, jbyteArray input_data) {
    jbyte *input_data_ptr;
    jbyte output_data[AMR_DECODED_FRAME_SIZE * 2];
    jbyteArray returnArray;
    input_data_ptr = (*env)->GetByteArrayElements(env, input_data, 0);
    if (amrnb_decode_frame((void *)h, (void *)input_data_ptr, (void*)output_data) != 0) {
	returnArray = (*env)->NewByteArray(env, (jsize)(AMR_DECODED_FRAME_SIZE * 2));
	(*env)->SetByteArrayRegion(env, returnArray, 0, (jsize)(AMR_DECODED_FRAME_SIZE * 2), output_data);
    } 
    (*env)->ReleaseByteArrayElements(env, input_data, input_data_ptr, 0);
    return returnArray;
}


/*------------------------------------------------------Encoder-------------------------------------------------------------------*/
JNIEXPORT jlong JNICALL Java_ru_maximgorin_media_amr_NbEncoder_initEncoder  (JNIEnv *env, jclass class, jint dtx) {
    void *h  = Encoder_Interface_init(dtx);
    return (jlong)h;
}

JNIEXPORT void JNICALL Java_ru_maximgorin_media_amr_NbEncoder_releaseEncoder (JNIEnv *env, jclass class, jlong h) {
    Encoder_Interface_exit((void*)h);
}

JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_NbEncoder_encodeAll (JNIEnv *env, jclass class, jint mode, jint dtx, jbyteArray input_data) {
    long output_data_len;
    jbyte *input_data_ptr;
    jbyte *output_data_ptr;
    jsize input_data_len;
    jbyteArray return_array;
    BufferDescriptor *bm_handler;
    int error_code;
    bm_handler = bm_create(2048);/*buffer size*/
    if (bm_handler == 0) {
	return 0;
    }
    input_data_len = (*env)->GetArrayLength(env, input_data);
    input_data_ptr = (*env)->GetByteArrayElements(env, input_data, 0);
    error_code = amrnb_encode(bm_handler, mode, dtx, (void*)input_data_ptr, (long)input_data_len);
    if (error_code != OK) {
	return 0;/*must be thrown an exception*/
    }
    output_data_len = bm_get_length(bm_handler);/*in bytes*/
    if (output_data_len >= 0) {
	return_array = (*env)->NewByteArray(env, (jsize)output_data_len);
	output_data_ptr = (*env)->GetByteArrayElements(env, return_array, 0);
	bm_to_array(bm_handler, output_data_ptr);
	(*env)->SetByteArrayRegion(env, return_array, 0, (jsize)output_data_len, (jbyte*)output_data_ptr);
	(*env)->ReleaseByteArrayElements(env, return_array, output_data_ptr, 0);
    }
    (*env)->ReleaseByteArrayElements(env, input_data, input_data_ptr, 0);
    bm_release(bm_handler);
    return return_array;
}

JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_NbEncoder_encodeFrame (JNIEnv *env, jclass class, jint mode, jlong h, jbyteArray input_data) {
    jbyte *input_data_ptr;
    jbyte output_data[500];
    long output_data_len;
    jbyteArray returnArray;
    input_data_ptr = (*env)->GetByteArrayElements(env, input_data, 0);
    if (amrnb_encode_frame(mode, (void *)h, (void *)input_data_ptr, (void*)output_data, &output_data_len) != 0) {
	returnArray = (*env)->NewByteArray(env, (jsize)output_data_len);
	(*env)->SetByteArrayRegion(env, returnArray, 0, (jsize)output_data_len, output_data);
    } 
    (*env)->ReleaseByteArrayElements(env, input_data, input_data_ptr, 0);
    return returnArray;
}