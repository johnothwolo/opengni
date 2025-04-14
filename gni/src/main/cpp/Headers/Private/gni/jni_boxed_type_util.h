//
//  jni_boxed_type_util.hpp
//  gni
//
//  Created by John Othwolo on 4/8/25.
//

#ifndef jni_boxed_type_util_hpp
#define jni_boxed_type_util_hpp

#include <jni.h>
#include <stdint.h>

namespace gni::common::jni_boxed_type_util {
bool     GetBoolean(jobject object);
int8_t   GetByte(jobject object);
uint8_t  GetCharacter(jobject object);
short    GetShort(jobject object);
int32_t  GetInteger(jobject object);
int64_t  GetLong(jobject object);
float    GetFloat(jobject object);
double   GetDouble(jobject object);
};

#endif /* jni_boxed_type_util_hpp */
