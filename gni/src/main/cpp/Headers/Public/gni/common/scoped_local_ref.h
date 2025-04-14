/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GNI_COMMON_SCOPED_LOCAL_REF_H_
#define GNI_COMMON_SCOPED_LOCAL_REF_H_

#include <jni.h>

namespace gni {
namespace common {

// A smart pointer that deletes a JNI local reference when it goes out of scope.
template <typename T>
class ScopedLocalRef {
 public:
  ScopedLocalRef(JNIEnv* env, T local_ref) : env_(env), local_ref_(local_ref) {}
  ScopedLocalRef(const ScopedLocalRef&) = delete;
  ScopedLocalRef& operator=(const ScopedLocalRef&) = delete;
  ~ScopedLocalRef() { Reset(); }

  void Reset(T ptr = nullptr) {
    if (ptr != local_ref_) {
      if (local_ref_ != nullptr) {
        env_->DeleteLocalRef(local_ref_);
      }
      local_ref_ = ptr;
    }
  }

  T Release() __attribute__((warn_unused_result)) {
    T local_ref = local_ref_;
    local_ref_ = nullptr;
    return local_ref;
  }

  T Get() const { return local_ref_; }

 private:
  JNIEnv* const env_;
  T local_ref_;
};

}  // namespace common
}  // namespace gni

#endif  // GNI_COMMON_SCOPED_LOCAL_REF_H_
