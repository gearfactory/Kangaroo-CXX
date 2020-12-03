#ifndef KANGAROON_INLCUDE_LOG_LEVEL_H_
#define KANGAROON_INLCUDE_LOG_LEVEL_H_
#include <stdint.h>

namespace zoo {
namespace kangaroon {
enum class LogLevel : uint8_t {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
};
}  // namespace kangaroon

}  // namespace zoo

#endif