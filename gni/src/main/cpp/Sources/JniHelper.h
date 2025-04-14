//
//  JniHelper.h
//  gni
//
//  Created by John Othwolo on 3/30/25.
//

#ifndef JniHelper_h
#define JniHelper_h

#include <atomic>
#include <pthread.h>
#include <jni.h>

namespace gni {
namespace common {

class JniHelper {
public:
    static JniHelper *GetInstance(JavaVM *jvm = NULL, jobject android_context = NULL);
    virtual jclass GetClass(char const* class_name) = 0;
    virtual jclass GetClass(JNIEnv *env,char const*class_name) = 0;
    virtual JNIEnv* GetEnv(void) = 0;
    virtual jclass GetClassGlobalRef(char const *class_name) = 0;
};

}; // namespace common
}; // namespace gni

#endif /* JniHelper_h */
