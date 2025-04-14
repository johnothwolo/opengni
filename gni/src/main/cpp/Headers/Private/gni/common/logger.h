#ifndef GNI_COMMON_LOGGER_H_
#define GNI_COMMON_LOGGER_H_

namespace gni {
namespace common {

void LogD(const char* tag, const char* format, ...);
void LogI(const char* tag, const char* format, ...);
void LogW(const char* tag, const char* format, ...);
void LogE(const char* tag, const char* format, ...);

}  // namespace common
}  // namespace gni

#endif  // GNI_COMMON_LOGGER_H_
