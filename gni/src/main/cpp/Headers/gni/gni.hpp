#ifndef CPP_GNI_H_
#define CPP_GNI_H_

#include <jni.h>
#include <string>

#include "gni/object.hpp"

namespace gni {

/// A singleton class that provides common operations used in a generated C and
/// C++ code.
class GniCore {
 public:
  /// Initializes the GNI runtime. Call this method before calling other
  /// methods.
  static void Init(JavaVM *java_vm, jobject android_context);

  /// Returns the instance of GniCore. Init() must be called before the first
  /// call of this method.
  static GniCore *GetInstance();

  virtual ~GniCore() = default;

  /// Returns a global reference to a jclass for the specified class name, or
  /// nullptr if the class isn't found.
  virtual jclass GetClassGlobalRef(const char *class_name) const;

  /// Returns a JNIEnv pointer appropriate for the current thread.
  virtual JNIEnv *GetJniEnv() const;

  /// Converts jstring to C-string. The returned string must be destroyed with
  /// GniString_destroy() function.
  virtual const char *ConvertString(jstring rhs) const;
  virtual std::string ConvertStringCPP(jstring rhs) const;

  /// Converts jobject to C-string. The returned string must be destroyed with
  /// GniString_destroy() function.
  virtual const char *ConvertString(jobject rhs) const;
  virtual std::string ConvertStringCPP(jobject rhs) const;

  /// Converts C-string to Java string represented by Object.
  virtual Object ConvertString(const char *rhs) const;

  /// Formats a URI as a string, so that an android.net.Uri isn't needed. The
  /// returned string must be destroyed with GniString_destroy() function.
  virtual const char *ConvertUri(jobject rhs) const;

  /// jobject to Object conversion. This is done as GNI uses a jobject wrapper
  /// to aide in type related logic.
  virtual Object Convert(jobject rhs) const;

 protected:
  GniCore() = default;

 private:
  GniCore(const GniCore &) = delete;
  GniCore(GniCore &&) = delete;
  GniCore &operator=(const GniCore &) = delete;
  GniCore &operator=(GniCore &&) = delete;
};

}  // namespace gni

#endif  // CPP_GNI_H_
