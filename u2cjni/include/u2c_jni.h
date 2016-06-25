/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class u2c_U2C */

#ifndef _Included_u2c_U2C
#define _Included_u2c_U2C
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     u2c_U2C
 * Method:    nativeInit
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_u2c_U2C_nativeInit
  (JNIEnv *, jobject);

/*
 * Class:     u2c_U2C
 * Method:    nativeDeinit
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_u2c_U2C_nativeDeinit
  (JNIEnv *, jobject, jlong);

/*
 * Class:     u2c_U2C
 * Method:    nativeOpen
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_u2c_U2C_nativeOpen
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     u2c_U2C
 * Method:    nativeClose
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_u2c_U2C_nativeClose
  (JNIEnv *, jobject, jlong);

/*
 * Class:     u2c_U2C
 * Method:    nativeSetSpeed
 * Signature: (JIIII)Z
 */
JNIEXPORT jboolean JNICALL Java_u2c_U2C_nativeSetSpeed
  (JNIEnv *, jobject, jlong, jint, jint, jint, jint);

/*
 * Class:     u2c_U2C
 * Method:    nativeSetFilter
 * Signature: (JIIIII)Z
 */
JNIEXPORT jboolean JNICALL Java_u2c_U2C_nativeSetFilter
  (JNIEnv *, jobject, jlong, jint, jint, jint, jint, jint);

/*
 * Class:     u2c_U2C
 * Method:    nativeSetMode
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_u2c_U2C_nativeSetMode
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     u2c_U2C
 * Method:    nativeReadData
 * Signature: (J)[Lu2c/CanFrame;
 */
JNIEXPORT jobjectArray JNICALL Java_u2c_U2C_nativeReadData
  (JNIEnv *, jobject, jlong);

/*
 * Class:     u2c_U2C
 * Method:    nativeSendData
 * Signature: (JLu2c/CanFrame;)Z
 */
JNIEXPORT jboolean JNICALL Java_u2c_U2C_nativeSendData
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     u2c_U2C
 * Method:    nativeGetRecvFifoUsage
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_u2c_U2C_nativeGetRecvFifoUsage
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
