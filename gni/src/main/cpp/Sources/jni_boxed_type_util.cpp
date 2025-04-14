//
//  jni_boxed_type_util.cpp
//  gni
//
//  Created by John Othwolo on 4/8/25.
//

#include "gni/gni.hpp"
#include "gni/common/logger.h"
#include "gni/interface_proxy.h"
#include "gni/common/jni_boxed_type_util.h"
#include "helper_macros.h"

#define TAG "JniBoxedType"

namespace gni::common {

template<typename CType>
struct JniBoxedType
{
    typedef CType (*CallGetterMethodType)(JNIEnv *, jobject, jmethodID, ...);
    CType (JNIEnv::*CallGetterMethod)(JNIEnv *, jobject, jmethodID, ...);
    jclass Class;
    jmethodID GetterMethodID;
};

#define DEFINE_GETTER(cValueType, javaType, JniGetter, javaTypeClassPath, javaGetter, javaGetterSignature)\
    cValueType jni_boxed_type_util::Get##javaType(jobject jobj) {\
        static const JniBoxedType<cValueType> *boxed_type;\
        JniBoxedType<cValueType>::CallGetterMethodType Getter;\
        GniCore* gniCore;\
        JNIEnv *env;\
    __Require_Msg(jobj, TAG, "Invalid instance reference.");\
    gniCore = gni::GniCore::GetInstance();\
    __Require_Msg(gniCore, TAG, "GniCore is NULL.");\
    env = gniCore->GetJniEnv();\
    boxed_type = new JniBoxedType<cValueType>{\
        .CallGetterMethod = (__typeof(boxed_type->CallGetterMethod))&JNIEnv::JniGetter,\
        .Class = gniCore->GetClassGlobalRef(javaTypeClassPath),\
        .GetterMethodID = env->GetMethodID(boxed_type->Class, javaGetter, javaGetterSignature),\
    };\
    __Require_Msg(boxed_type, TAG, "No memory");\
    __Require_Msg(boxed_type->Class, TAG, "Class reference invalid");\
    Getter = (JniBoxedType<cValueType>::CallGetterMethodType) &boxed_type->CallGetterMethod;\
    return Getter(env, jobj, boxed_type->GetterMethodID);\
}\

DEFINE_GETTER(bool,    Boolean,   CallByteMethod,   "java/lang/Boolean",   "booleanValue", "()Z");
DEFINE_GETTER(int8_t,  Byte,      CallByteMethod,   "java/lang/Byte",      "byteValue",    "()B");
DEFINE_GETTER(uint8_t, Character, CallCharMethod,   "java/lang/Character", "charValue",    "()C");
DEFINE_GETTER(short,   Short,     CallShortMethod,  "java/lang/Short",     "shortValue",   "()S");
DEFINE_GETTER(int32_t, Integer,   CallIntMethod,    "java/lang/Integer",   "intValue",     "()I");
DEFINE_GETTER(int64_t, Long,      CallLongMethod,   "java/lang/Long",      "longValue",    "()J");
DEFINE_GETTER(float,   Float,     CallFloatMethod,  "java/lang/Float",     "floatValue",   "()F");
DEFINE_GETTER(double,  Double,    CallDoubleMethod, "java/lang/Double",    "doubleValue",  "()D");

/*
 // Reference Method
bool
jni_boxed_type_util::GetBoolean(jobject jobj)
{
    static const JniBoxedType<bool> *boxed_type;
    JniBoxedType<bool>::CallGetterMethodType Getter;
    GniCore* gniCore;
    JNIEnv *env;
    
    __Require_Msg(jobj, "JniBoxedType", "Invalid instance reference.");
    
    gniCore = gni::GniCore::GetInstance();
    __Require_Msg(gniCore, "JniBoxedType", "GniCore is NULL.");
    env = gniCore->GetJniEnv();
    boxed_type = new JniBoxedType<bool>{
        .CallGetterMethod = (__typeof(boxed_type->CallGetterMethod))&JNIEnv_::CallBooleanMethod,
        .Class = gniCore->GetClassGlobalRef("java/lang/Boolean"),
        .GetterMethodID = env->GetMethodID(boxed_type->Class, "booleanValue", "()Z"),
    };
    __Require_Msg(boxed_type, "JniBoxedType", "No memory");
    __Require_Msg(boxed_type->Class, "JniBoxedType", "Class reference invalid");
    
    Getter = (JniBoxedType<bool>::CallGetterMethodType) &boxed_type->CallGetterMethod;
    return Getter(env, jobj, boxed_type->GetterMethodID);
}
*/

};
