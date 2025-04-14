#ifndef CPP_GNI_COMMON_INTERFACE_PROXY_H_
#define CPP_GNI_COMMON_INTERFACE_PROXY_H_

#include <jni.h>
#include <stdint.h>

namespace gni {
namespace common {
namespace interface_proxy {

// A definition of the callback function to which Java calls will be redirected.
// The `object_pointer` is a pointer to the corresponding struct in the
// generated code. The `method_name` is a name of the method that Java calls.
// The `arguments` is an array of arguments passed to `method_name` method. The
// `arguments_count` is the length of the `arguments` array.
typedef jobject (*Callback)(void* object_pointer, const char* method_name,
                            jobject* arguments, uint32_t arguments_count);

// Creates an instance of the Dynamic Proxy Class that implements the
// `java_interface_name` interface. All calls to the `java_interface_name`
// interface on the Java side will be redirected to the `callback` function.
// The `object_pointer` is a pointer to the corresponding struct and will be
// provided to the `callback` function as the first argument.
jobject RegisterCallback(const char* java_interface_name, void* object_pointer,
                         Callback callback);

}  // namespace interface_proxy
}  // namespace common
}  // namespace gni

#endif  // namespace CPP_GNI_COMMON_INTERFACE_PROXY_H_
