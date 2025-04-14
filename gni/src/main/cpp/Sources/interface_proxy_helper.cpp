//
//  interface_proxy_helper.cpp
//  gni
//
//  Created by John Othwolo on 4/8/25.
//

#include <jni.h>
#include <stdint.h>

#include "gni/gni.hpp"
#include "gni/common/logger.h"
#include "helper_macros.h"

#define TAG "InterfaceProxyNative"

namespace gni::common::interface_proxy {

typedef jobject (*Callback)(void *, const char *, jobject *, uint32_t);

jobject
NativeInvoke(JNIEnv *_env,
             jobject object,
             jlong c_object,
             Callback c_callback,
             jstring java_method_name,
             jobjectArray java_arguments);

const JNINativeMethod nativeMethods[] = {
    { "nativeInvoke", "(JJLjava/lang/String;[Ljava/lang/Object;)Ljava/lang/Object;", (void*)NativeInvoke }
};

jobject RegisterCallback(const char *java_interface_name, void *c_object_pointer, Callback callback)
{
    GniCore* gniCore = gni::GniCore::GetInstance();
    __Require_Msg(gniCore, TAG, "GniCore is NULL.");
    JNIEnv *env = gniCore->GetJniEnv();
    __Require_Msg(env, TAG, "JNIEnv is NULL.");
    static const jclass iphClass = gniCore->GetClassGlobalRef("com/librarywrapper/core/InterfaceProxyHelper");
    __Require_Msg(iphClass, TAG, "Couldn't find class InterfaceProxyHelper.");
    
    __Require_Msg(env->RegisterNatives(iphClass, nativeMethods, sizeof(nativeMethods)/sizeof(JNINativeMethod)),
                  TAG, "Failed to register natives.");
    
    static const jmethodID iphCreateMethodID = env->GetStaticMethodID(iphClass, "createInterfaceProxy", "(JJLjava/lang/Class;)Ljava/lang/Object;");
    __Require_Msg(iphCreateMethodID, TAG, "Couldn't get a nativeInvoke method id.");
    
    jclass java_interface_classRef = gniCore->GetClassGlobalRef(java_interface_name);
    __Require_Msg(java_interface_classRef, TAG, "Couldn't find interface %s.", java_interface_name);
    
    jobject resultObj = env->CallStaticObjectMethod(iphClass,
                                                    iphCreateMethodID,
                                                    c_object_pointer,
                                                    callback,
                                                    java_interface_classRef);
    __Require_Msg(resultObj, TAG, "A call to method %s failed.", "createInterfaceProxy");
    return resultObj;
}


jobject
NativeInvoke(JNIEnv *_env,
             jobject object,
             jlong c_object,
             Callback c_callback,
             jstring java_method_name,
             jobjectArray java_arguments)
{
    GniCore* gniCore;
    JNIEnv *env;
    int arrayLen = 0;
    jobject* objectList, resultObj;
    const char *utf8String;
    
    gniCore = gni::GniCore::GetInstance();
    __Require_Msg(gniCore, TAG, "GniCore is NULL.");
    env = gniCore->GetJniEnv();
    __Require_Msg(env, TAG, "JNIEnv is NULL.");
    arrayLen = env->GetArrayLength(java_arguments);
    objectList = nullptr;
    
    if (arrayLen > 0){
        objectList = (jobject*) new char[sizeof(__SIZEOF_POINTER__) * arrayLen];
        if (java_arguments && objectList) {
            for (int i = 0; i != arrayLen; ++i)
                objectList[i] = env->GetObjectArrayElement(java_arguments, i);
        }
    }
    
    utf8String = env->GetStringUTFChars(java_method_name, 0);
    resultObj = c_callback((void*)c_object, utf8String, objectList, arrayLen);
    env->ReleaseStringUTFChars(java_method_name, utf8String);
    if (objectList && arrayLen > 0)
        operator delete[](objectList);
    
    return resultObj;
}

};
