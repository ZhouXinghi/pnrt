#ifndef PNRT_LOGGING_H
#define PNRT_LOGGING_H

#include <glog/logging.h>

namespace pnrt {

enum class LogSeverity {
  kInfo = 0,
  kWarn = 1,
  kError = 2,
  kFatal = 3,
};

#define PNRT_INFO Log(LogSeverity::kInfo)
#define PNRT_WARN Log(LogSeverity::kWarn)
#define PNRT_ERROR Log(LogSeverity::kError)
#define PNRT_FATAL Log(LogSeverity::kFatal)

inline decltype(LOG(INFO)) Log(LogSeverity severity) {
  switch (severity) {
    case LogSeverity::kInfo:
      return LOG(INFO);
    case LogSeverity::kWarn:
      return LOG(WARNING);
    case LogSeverity::kError:
      return LOG(ERROR);
    case LogSeverity::kFatal:
      return LOG(FATAL);
  }
}

#define PNRT_LOG(severity) Log(severity)
#define PNRT_CHECK(condition) CHECK(condition)
#define PNRT_LOG_IF(severity, condition) LOG_IF(severity, condition)

}  // namespace pnrt

#endif  // PNRT_LOGGING_H