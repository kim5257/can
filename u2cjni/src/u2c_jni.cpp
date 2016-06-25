/*
 * u2c_U2C.cpp
 *
 *  Created on: 2015. 12. 28.
 *      Author: arun
 */

#include <u2c.h>
#include "u2c_jni.h"

/*
 * Class:     u2c_U2C
 * Method:    nativeInit
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL
Java_u2c_U2C_nativeInit (	JNIEnv *env,
							jobject obj)
{
	jlong u2cObjPtr = (jlong)new u2c::U2C ();
	return u2cObjPtr;
}

/*
 * Class:     u2c_U2C
 * Method:    nativeDeinit
 * Signature: (J)V
 */
JNIEXPORT void JNICALL
Java_u2c_U2C_nativeDeinit (	JNIEnv *env,
							jobject obj,
							jlong objPtr)
{
	u2c::U2C* u2cObjPtr = (u2c::U2C*)objPtr;
	delete u2cObjPtr;
}

/*
 * Class:     u2c_U2C
 * Method:    nativeOpen
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL
Java_u2c_U2C_nativeOpen (	JNIEnv *env,
							jobject obj,
							jlong u2cObj,
							jint idx)
{
	jboolean ret = JNI_FALSE;

	try
	{
		u2c::U2C* u2cObjPtr = (u2c::U2C*)u2cObj;
		u2cObjPtr->open (idx);

		ret = JNI_TRUE;
	}
	catch( error::ErrMsg* err )
	{
		error::ErrMsg::destroyErrMsg (err);
	}

	return ret;
}

/*
 * Class:     u2c_U2C
 * Method:    nativeClose
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_u2c_U2C_nativeClose (	JNIEnv *env,
							jobject obj,
							jlong u2cObj)
{
	jboolean ret = JNI_FALSE;

	try
	{
		u2c::U2C* u2cObjPtr = (u2c::U2C*)u2cObj;
		u2cObjPtr->close ();

		ret = JNI_TRUE;
	}
	catch( error::ErrMsg* err )
	{
		error::ErrMsg::destroyErrMsg (err);
	}

	return ret;
}

/*
 * Class:     u2c_U2C
 * Method:    nativeSetSpeed
 * Signature: (IIII)Z
 */
JNIEXPORT jboolean JNICALL
Java_u2c_U2C_nativeSetSpeed (	JNIEnv *env,
								jobject obj,
								jlong u2cObj,
								jint bs1,
								jint bs2,
								jint swj,
								jint prescaler)
{
	jboolean ret = JNI_FALSE;

	try
	{
		u2c::U2C* u2cObjPtr = (u2c::U2C*)u2cObj;
		u2cObjPtr->setSpeed (bs1, bs2, swj, prescaler);

		ret = JNI_TRUE;
	}
	catch( error::ErrMsg* err )
	{
		error::ErrMsg::destroyErrMsg (err);
	}

	return ret;
}

/*
 * Class:     u2c_U2C
 * Method:    nativeSetFilter
 * Signature: (IIIII)Z
 */
JNIEXPORT jboolean JNICALL
Java_u2c_U2C_nativeSetFilter (	JNIEnv *env,
								jobject obj,
								jlong u2cObj,
								jint active,
								jint idx,
								jint idType,
								jint mask,
								jint filter)
{
	jboolean ret = JNI_FALSE;

	try
	{
		u2c::U2C* u2cObjPtr = (u2c::U2C*)u2cObj;

		printf ("filter = %d %d %d %x %x\n", active, idx, idType, mask, filter);

		u2cObjPtr->setFilter (active, idx, idType, mask, filter);

		ret = JNI_TRUE;
	}
	catch( error::ErrMsg* err )
	{
		error::ErrMsg::destroyErrMsg (err);
	}

	return ret;
}

/*
 * Class:     u2c_U2C
 * Method:    nativeSetMode
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL
Java_u2c_U2C_nativeSetMode (JNIEnv *env,
							jobject obj,
							jlong u2cObj,
							jint mode)
{
	jboolean ret = JNI_FALSE;

	try
	{
		u2c::U2C* u2cObjPtr = (u2c::U2C*)u2cObj;
		u2cObjPtr->setMode (mode);

		ret = JNI_TRUE;
	}
	catch( error::ErrMsg* err )
	{
		error::ErrMsg::destroyErrMsg (err);
	}

	return ret;
}

/*
 * Class:     u2c_U2C
 * Method:    nativeReadData
 * Signature: ([Lu2c/CanFrame;)Z
 */
JNIEXPORT jobjectArray JNICALL
Java_u2c_U2C_nativeReadData (	JNIEnv *env,
								jobject obj,
								jlong u2cObj)
{
	jboolean ret = JNI_FALSE;
	jobjectArray frameArray = NULL;

	u2c::data_vector_t data;

	try
	{
		u2c::U2C* u2cObjPtr = (u2c::U2C*)u2cObj;
		u2cObjPtr->readData (data);

		ret = JNI_TRUE;
	}
	catch( error::ErrMsg* err )
	{
		error::ErrMsg::destroyErrMsg (err);
	}

	if ( ret != JNI_FALSE )
	{
		jclass jframe = env->FindClass ("Lu2c/CanFrame;");

		// CanFrame 배열 생성
		frameArray = env->NewObjectArray (data.size (), jframe, NULL);

		// CanFrame 생성자 식별자 가져오기
		jmethodID init = env->GetMethodID (jframe, "<init>", "(III[B)V");

		for(size_t idx = 0;idx < data.size ();++idx)
		{
			// Frame의 Data 배열 생성 및 데이터 넣기
			jbyteArray dataArray = env->NewByteArray (data[idx].mLen);
			env->SetByteArrayRegion (	dataArray,
										0,
										data[idx].mLen,
										(jbyte*)data[idx].mData);

			jobject newObj = env->NewObject (	jframe,
												init,
												data[idx].mExt,
												data[idx].mId,
												data[idx].mLen,
												dataArray);
			env->SetObjectArrayElement (frameArray, idx, newObj);
		}
	}

	return frameArray;
}

/*
 * Class:     u2c_U2C
 * Method:    nativeSendData
 * Signature: (Lu2c/CanFrame;)Z
 */
JNIEXPORT jboolean JNICALL
Java_u2c_U2C_nativeSendData (	JNIEnv *env,
								jobject obj,
								jlong u2cObj,
								jobject frameObj)
{
	jboolean ret = JNI_FALSE;
	u2c::CanFrame frame =
	{ 0, };
	jclass jframe = env->GetObjectClass (frameObj);

	jmethodID getExt = env->GetMethodID (jframe, "getExt", "()I");
	jmethodID getId = env->GetMethodID (jframe, "getId", "()I");
	jmethodID getLen = env->GetMethodID (jframe, "getLen", "()I");
	jmethodID getData = env->GetMethodID (jframe, "getData", "()[B");

	frame.mExt = env->CallIntMethod (frameObj, getExt);
	frame.mId = env->CallIntMethod (frameObj, getId);
	frame.mLen = env->CallIntMethod (frameObj, getLen);

	jbyteArray dataArray = (jbyteArray)env->CallObjectMethod (	frameObj,
																getData);
	jbyte* data = env->GetByteArrayElements (dataArray, NULL);

	memcpy (frame.mData, data, sizeof(char) * frame.mLen);

	env->ReleaseByteArrayElements (dataArray, data, 0);

	try
	{
		u2c::U2C* u2cObjPtr = (u2c::U2C*)u2cObj;
		u2cObjPtr->sendData (frame);

		printf ("%x %x %x ", frame.mId, frame.mExt, frame.mLen);
		for(int i = 0;i < 8;i++)
		{
			printf ("%02x ", (unsigned char)frame.mData[i]);
		}
		printf ("\n");

		ret = JNI_TRUE;
	}
	catch( error::ErrMsg* err )
	{
		error::ErrMsg::destroyErrMsg (err);
	}

	return ret;
}

/*
 * Class:     u2c_U2C
 * Method:    nativeGetRecvFifoUsage
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL
Java_u2c_U2C_nativeGetRecvFifoUsage (	JNIEnv *env,
										jobject obj,
										jlong u2cObj)
{
	u2c::U2C* u2cObjPtr = (u2c::U2C*)u2cObj;

	return (jlong)u2cObjPtr->getRecvFifoUsage();
}
