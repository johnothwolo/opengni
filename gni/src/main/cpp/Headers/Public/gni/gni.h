#ifndef C_GNI_H_
#define C_GNI_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Simulates type casting by creating a new wrapper for the existing jni
/// reference. The returned pointer needs to be destoyed when no longer needed.
#define GNI_CAST(DST_TYPE, SRC_TYPE, SRC_POINTER) \
  DST_TYPE##_wrapJniReference(SRC_TYPE##_getJniReference(SRC_POINTER))

/// Initializes the GNI runtime. Call this function before calling other
/// GNI functions.
void GniCore_init(JavaVM* java_vm, jobject android_context);

/// Frees the specified string that was previously allocated by a GNI API call.
void GniString_destroy(const char* value);

#ifdef __cplusplus
};  // extern "C"
#endif

#endif  // C_GNI_H_
