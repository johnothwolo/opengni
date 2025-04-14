//
//  gni.cpp
//  gni
//
//  Created by John Othwolo on 3/30/25.
//

#include "gni/gni.h"
#include "gni/gni.hpp"
#include "gni/object.hpp"

#include "gni/common/logger.h"

#include "JniHelper.h"

#include <stdlib.h>
#include <pthread.h>
#include <atomic>

#include <android/log.h>

namespace gni {

static pthread_mutex_t lock;
static std::atomic_bool initialized;
static gni::GniCore *gni_core_instance;

void
GniCore_init(JavaVM* java_vm, jobject android_context)
{
    gni::GniCore::Init(java_vm, android_context);
}

void
GniString_destroy(const char* value)
{
    free((void*)value);
}

void
gni::GniCore::Init(JavaVM *java_vm, jobject android_context)
{
    gni::common::LogI("GniCore", "Initializing the GNI runtime.");
    pthread_mutex_lock(&lock);
    if (initialized.load() == false)
    {
        (void)gni::common::JniHelper::GetInstance(java_vm, android_context);
        initialized.store(true);
    }
    pthread_mutex_unlock(&lock);
}

GniCore *
gni::GniCore::GetInstance()
{
    pthread_mutex_lock(&lock);
    if (!gni_core_instance) {
        if (!initialized) {
            pthread_mutex_unlock(&lock);
            gni::common::LogE("GniCore", "Call GniCore::Init() first.");
            return NULL;
        }
        gni_core_instance = new gni::GniCore;
    }
    pthread_mutex_unlock(&lock);
    return gni_core_instance;
}

jclass
gni::GniCore::GetClassGlobalRef(const char *class_name) const
{
    return common::JniHelper::GetInstance()->GetClassGlobalRef(class_name);
}

JNIEnv *
gni::GniCore::GetJniEnv() const
{
    return common::JniHelper::GetInstance()->GetEnv();
}

const char *
gni::GniCore::ConvertString(jstring rhs) const
{
    JNIEnv *env = GetJniEnv();
    auto chars = env->GetStringUTFChars(rhs, nullptr);
    auto ret = strdup(chars);
    env->ReleaseStringUTFChars(rhs, chars);
    return ret;
}

// would be nice to also have a cpp string
std::string
gni::GniCore::ConvertStringCPP(jstring rhs) const
{
    JNIEnv *env = GetJniEnv();
    auto chars = env->GetStringUTFChars(rhs, nullptr);
    auto ret = std::string(chars);
    env->ReleaseStringUTFChars(rhs, chars);
    return ret;
}

const char *
gni::GniCore::ConvertString(jobject rhs) const
{
    return ConvertString((jstring) rhs);
}

std::string
gni::GniCore::ConvertStringCPP(jobject rhs) const
{
    return ConvertStringCPP((jstring)rhs);
}

Object
gni::GniCore::ConvertString(const char *rhs) const
{
    return gni::Object(GetJniEnv()->NewStringUTF(rhs));
}

const char *
gni::GniCore::ConvertUri(jobject rhs) const
{
    JNIEnv *env = GetJniEnv();
    static const jclass uri_class = GetClassGlobalRef("android/net/Uri");
    static const jmethodID to_string_method_id = env->GetMethodID(uri_class, "toString", "()Ljava/lang/String;");
    jobject str_object = env->CallObjectMethod(rhs, to_string_method_id);
    const char *str = ConvertString(str_object);
    env->DeleteLocalRef(str_object);
    return str;
}

Object
gni::GniCore::Convert(jobject rhs) const
{
    return gni::Object(rhs);
}

void
gni::common::LogD(const char* tag, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    __android_log_vprint(0, tag, format, ap);
    va_end(ap);
}

void
gni::common::LogI(const char* tag, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    __android_log_vprint(0, tag, format, ap);
    va_end(ap);
}

void
gni::common::LogW(const char* tag, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    __android_log_vprint(0, tag, format, ap);
    va_end(ap);
}

void
gni::common::LogE(const char* tag, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    __android_log_vprint(0, tag, format, ap);
    va_end(ap);
}

}; // namespace gni
