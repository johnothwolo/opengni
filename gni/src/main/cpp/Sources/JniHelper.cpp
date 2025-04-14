//
//  JniHelper.cpp
//  gni
//
//  Created by John Othwolo on 3/30/25.
//

#include <stdio.h>

#include "JniHelper.h"
#include "gni/common/logger.h"


namespace gni::common {

static pthread_key_t jni_env_key;
static pthread_once_t pthread_key_initialized;

static void
DetachThreadFromJvm(void *java_vm)
{
    jint res;
    JNIEnv *env = nullptr;
    JavaVM *jvm = (JavaVM *) java_vm;

    gni::common::LogD("JniHelper", "Detaching thread from JavaVM...");
    res = jvm->AttachCurrentThread(&env, nullptr);
    if (res) {
        gni::common::LogE("JniHelper", "Skipping JNI thread detach since attach status=%d", res);
    } else {
        jvm->DetachCurrentThread();
        gni::common::LogD("JniHelper", "Detached thread from JavaVM.");
    }
}

static void
SetupJvmDetachOnThreadDestruction(void)
{
    pthread_key_create(&jni_env_key, DetachThreadFromJvm);
}


class JniHelperImpl : public JniHelper {
    struct JavaClassLoader {
        jobject object;
        jmethodID load_class_mid;
    };
public:
    ~JniHelperImpl(){};
    bool Initialize(JavaVM * jvm, jobject android_context)
    {
        JNIEnv* env;
        jclass ContextClass;
        jmethodID getApplicationContextID, getClassLoader;
        jobject context;

        if (!jvm) {
            gni::common::LogE("JniHelper", "\"jvm\" must not be null.", android_context);
            return false;
        }

        jvm_ = jvm;
        if (!android_context) {
            gni::common::LogE("JniHelper", "\"android_context\" must not be null.", android_context);
            return false;
        }

        env = GetEnv();
        ContextClass = env->FindClass("android/content/Context");
        if (!ContextClass) {
            gni::common::LogE("JniHelper", "\"android_context\" class is null.", android_context);
            return false;
        }

        if (!env->IsInstanceOf(android_context, ContextClass)) {
            gni::common::LogE("JniHelper", "\"android_context\" must be an instance of android.content.Context.", android_context);
            return false;
        }

        ContextClass = env->GetObjectClass(android_context);
        getApplicationContextID = env->GetMethodID(ContextClass, "getApplicationContext", "()Landroid/content/Context;");
        context = env->CallObjectMethod(android_context, getApplicationContextID);
        getClassLoader = env->GetMethodID(ContextClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
        class_loader_.object = env->NewGlobalRef(env->CallObjectMethod(context, getClassLoader));
        class_loader_.load_class_mid = env->GetMethodID(env->GetObjectClass(class_loader_.object),
                                                        "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        return true;
    }

    jclass GetClass(char const* class_name) override
    {
        return GetClass(GetEnv(), class_name);
    }

    jclass GetClass(JNIEnv *env,char const*class_name) override
    {
        jthrowable exceptionOccured;
        jobject stringObject;
        jclass Class;

        stringObject = env->NewStringUTF(class_name);
        Class = (jclass)env->CallObjectMethod(class_loader_.object,
                                                      class_loader_.load_class_mid,
                                                      stringObject);
        env->DeleteLocalRef(stringObject);
        exceptionOccured = env->ExceptionOccurred();
        if (exceptionOccured) {
            env->ExceptionClear();
            Class = env->FindClass("java/lang/ClassNotFoundException");
            if (env->IsInstanceOf((jobject)exceptionOccured, Class))
                gni::common::LogE("JniHelper", "ClassNotFoundException: %s", class_name);
            else
                env->Throw(exceptionOccured);
            return nullptr;
        }
        return Class;
    }

    JNIEnv* GetEnv(void) override
    {
        jint retval;
        JNIEnv *env = nullptr;

        if (!jvm_->GetEnv((void**)&env, JNI_VERSION_1_6))
            return NULL;

        gni::common::LogD("JniHelper", "Attaching thread to JavaVM...");
        pthread_once(&pthread_key_initialized,
                     SetupJvmDetachOnThreadDestruction);
        pthread_setspecific(jni_env_key, jvm_);
        if ((retval = jvm_->AttachCurrentThread(&env, NULL)) != 0) {
            gni::common::LogE("JniHelper", "Failed to attach thread to JavaVM: %d", retval);
            return NULL;
        }

        gni::common::LogD("JniHelper", "Attached thread to JavaVM.");
        return env;
    }

    jclass GetClassGlobalRef(char const *class_name) override
    {
        JNIEnv *env = GetEnv();
        return (jclass) env->NewGlobalRef(GetClass(env, class_name));
    }

private:
    JavaVM *jvm_;
    JavaClassLoader class_loader_;
};

static pthread_mutex_t instance_lock_;
static std::atomic_bool instance_initialized_;
static JniHelperImpl instance_;

JniHelper*
JniHelper::GetInstance(JavaVM *jvm, jobject android_context)
{
    pthread_mutex_lock(&instance_lock_);
    if (instance_initialized_.load() == false)
        instance_initialized_.store(instance_.Initialize(jvm, android_context));
    pthread_mutex_unlock(&instance_lock_);
    if (instance_initialized_.load())
        return &instance_;
    return nullptr;
}

} // namespace gni::common
