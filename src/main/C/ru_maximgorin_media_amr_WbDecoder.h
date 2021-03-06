/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class ru_maximgorin_media_amr_WbDecoder */

#ifndef _Included_ru_maximgorin_media_amr_WbDecoder
#define _Included_ru_maximgorin_media_amr_WbDecoder
#ifdef __cplusplus
extern "C" {
#endif
#undef ru_maximgorin_media_amr_WbDecoder_AMRWB_FORMAT_FREQUENCY
#define ru_maximgorin_media_amr_WbDecoder_AMRWB_FORMAT_FREQUENCY 16000L
#undef ru_maximgorin_media_amr_WbDecoder_AMRWB_FORMAT_BITS_PER_SAMPLE
#define ru_maximgorin_media_amr_WbDecoder_AMRWB_FORMAT_BITS_PER_SAMPLE 16L
#undef ru_maximgorin_media_amr_WbDecoder_AMRWB_FORMAT_CHANNEL_COUNT
#define ru_maximgorin_media_amr_WbDecoder_AMRWB_FORMAT_CHANNEL_COUNT 1L
#undef ru_maximgorin_media_amr_WbDecoder_DECODED_BUFFER_SIZE
#define ru_maximgorin_media_amr_WbDecoder_DECODED_BUFFER_SIZE 320L
/*
 * Class:     ru_maximgorin_media_amr_WbDecoder
 * Method:    initDecoder
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_ru_maximgorin_media_amr_WbDecoder_initDecoder
  (JNIEnv *, jclass);

/*
 * Class:     ru_maximgorin_media_amr_WbDecoder
 * Method:    releaseDecoder
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_ru_maximgorin_media_amr_WbDecoder_releaseDecoder
  (JNIEnv *, jclass, jlong);

/*
 * Class:     ru_maximgorin_media_amr_WbDecoder
 * Method:    decodeAll
 * Signature: ([B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_WbDecoder_decodeAll
  (JNIEnv *, jclass, jbyteArray);

/*
 * Class:     ru_maximgorin_media_amr_WbDecoder
 * Method:    decodeFrame
 * Signature: (J[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_WbDecoder_decodeFrame
  (JNIEnv *, jclass, jlong, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif
