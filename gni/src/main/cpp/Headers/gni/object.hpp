#ifndef CPP_GNI_OBJECT_H_
#define CPP_GNI_OBJECT_H_

#include <jni.h>

namespace gni {

// Base class for all JNI wrapped classes. Not intended for direct use by API
// consumers, but only to consolidate all references to a single place, to help
// with reference counting, debugging, and smart pointer management.

class Object {
 public:
  // Initializes a new instance of an Object class. A new global reference will
  // be created from the object and stored internally. The reference will be
  // deleted upon the destruction of the Object. The object can't be null.
  explicit Object(jobject object);

  // Move ctor is required for pre-C++17 compilers, which don't guarantee RVO.
  // TODO(b/213641316): Delete move ctor from Object.
  Object(Object&& object);

  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;
  Object& operator=(Object&&) = delete;

  virtual ~Object();

  // Returns a JNI reference represented by this object.
  virtual jobject GetImpl() const { return object_; }

 protected:
  virtual void SetImpl(jobject object) { this->object_ = object; }

 private:
  jobject object_;
};

}  // namespace gni

#endif  // CPP_GNI_OBJECT_H_
