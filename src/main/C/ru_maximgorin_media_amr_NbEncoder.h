/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class ru_maximgorin_media_amr_NbEncoder */

#ifndef _Included_ru_maximgorin_media_amr_NbEncoder
#define _Included_ru_maximgorin_media_amr_NbEncoder
#ifdef __cplusplus
extern "C" {
#endif
#undef ru_maximgorin_media_amr_NbEncoder_DECODED_BUFFER_SIZE
#define ru_maximgorin_media_amr_NbEncoder_DECODED_BUFFER_SIZE 320L
/*
 * Class:     ru_maximgorin_media_amr_NbEncoder
 * Method:    initEncoder
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_ru_maximgorin_media_amr_NbEncoder_initEncoder
  (JNIEnv *, jclass, jint);

/*
 * Class:     ru_maximgorin_media_amr_NbEncoder
 * Method:    releaseEncoder
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_ru_maximgorin_media_amr_NbEncoder_releaseEncoder
  (JNIEnv *, jclass, jlong);

/*
 * Class:     ru_maximgorin_media_amr_NbEncoder
 * Method:    encodeAll
 * Signature: (II[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_NbEncoder_encodeAll
  (JNIEnv *, jclass, jint, jint, jbyteArray);

/*
 * Class:     ru_maximgorin_media_amr_NbEncoder
 * Method:    encodeFrame
 * Signature: (IJ[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_ru_maximgorin_media_amr_NbEncoder_encodeFrame
  (JNIEnv *, jclass, jint, jlong, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif
