#include "log.h"

#include <string>
#include <vector>

#include "log_level.h"
#include "time_stamp.h"

namespace zoo {
namespace kangaroon {
namespace {
std::string getLogLevelStr(LogLevel log_level) {
    switch (level) {
#define XX(name)         \
    case LogLevel::name: \
        return #name;    \
        break;

        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
#undef XX
        default:
            return "UNKNOW";
    }
    return "UNKNOW";
}
}  // namespace

void Logger::info(const char *format, ...) {
    if (!format) {
        return;
    }
    va_list ap;

    va_start(ap, format);
    writeLog(LogLevel::INFO, __FILE__, __FUNCTION__, __LINE__, format, ap);
    va_end(ap);
}
void Logger::debug(const char *format, ...) {
    if (!format) {
        return;
    }
    va_list ap;

    va_start(ap, format);
    writeLog(LogLevel::DEBUG, __FILE__, __FUNCTION__, __LINE__, format, ap);
    va_end(ap);
}
void Logger::warn(const char *format, ...) {
    if (!format) {
        return;
    }
    va_list ap;

    va_start(ap, format);
    writeLog(LogLevel::WARN, __FILE__, __FUNCTION__, __LINE__, format, ap);
    va_end(ap);
}
void Logger::error(const char *format, ...) {
    if (!format) {
        return;
    }
    va_list ap;

    va_start(ap, format);
    writeLog(LogLevel::ERROR, __FILE__, __FUNCTION__, __LINE__, format, ap);
    va_end(ap);
}
void Logger::fatal(const char *format, ...) {
    if (!format) {
        return;
    }
    va_list ap;

    va_start(ap, format);
    writeLog(LogLevel::FATAL, __FILE__, __FUNCTION__, __LINE__, format, ap);
    va_end(ap);
}
/*需要加锁的*/
void Logger::addAppender(const std::string &appender_name,
                         LogAppenderInterface::Ptr appender) {
        MutexGuard guard(mutex_);
    appenders_[appender_name] = appender;
}
void Logger::delAppender(const std::string& appender_name) {
    MutexGuard guard(mutex_);
    auto&& iter = reomve_if(appenders.begin(),appenders.end(),[&appender_name](const std::string name){
        return appender_name == name;
    })
     
    appenders.erase(iter);
     
}
void Logger::clearAppender() {
    appenders.clear();
}

void Logger::writeLog(LogLevel log_level, const char *file_name,
                      const char *function_name, int32_t line_num,
                      const char *fmt, va_list ap) {
    if (log_level < kLogConfig.log_level) {
        return;
    }
    std::string str_result;
    if (NULL != fmt) {
        va_list marker = NULL;
        va_start(marker, fmt);                        //初始化变量参数
        size_t length = _vscprintf(fmt, marker) + 1;  //获取格式化字符串长度
        std::vector<char> fmt_bufs(length,
                                   '\0');  //创建用于存储格式化字符串的字符数组
        int writen_n = _vsnprintf_s(&fmt_bufs[0], fmt_bufs.size(), length,
                                    writen_n, marker);
        if (writen_n > 0) {
            str_result = &fmt_bufs[0];
        }
        va_end(marker);  //重置变量参数
    }
    if (str_result.empty()) {
        return;
    }
    auto getSourceFileName = [](const char *file_name) {
        return strrchr(file_name, '/') ? strrchr(file_name, '/') + 1
                                       : file_name;
    };
    std::string prefix;
    prefix.append(Timestamp.nowStrTime() + "-");
    prefix.append(getLogLevelStr(log_level) + "-");
    prefix.append(getSourceFileName(file_name) + "-");
    prefix.append(function_name + "-");
    prefix.append(std::to_string(line_num) + "-");
    prefix.append(str_result);
    MutexGuard guard(mutex_);
    for (const auto &appender : appenders_) {
        appender.append(prefix.data(), prefix.size());
    }
}
}  // namespace kangaroon

}  // namespace zoo