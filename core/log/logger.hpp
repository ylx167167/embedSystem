
#ifndef CORE_LOG_LOGGER_HPP_
#define CORE_LOG_LOGGER_HPP_
#if defined(_MSC_VER)
#pragma once
#elif defined(__GNUC__)
#if (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ >= 4)
#pragma once
#endif
#endif

#include <pthread.h>
#include <syslog.h>

#include <cstdio>
#include <sstream>

#define SYSLOG_OUTPUT_TAG "wayneyao"
// 使用格式化文本类printf输出日志
#if 0
// #include <string.h>  // strrchr()函数所需头文件
#include <cstring>
#define __FILENAME__ \
  (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

#define LOG_PRINTF(LEVEL, MSG, ...)                                     \
  syslog(LEVEL, "\"%s\"<%s>(%lu)\x20" MSG "\n", __FILENAME__, __func__, \
         pthread_self(), ##__VA_ARGS__)

#else
#define LOG_PRINTF(LEVEL, MSG, ...)                                    \
  syslog(LEVEL, "[%lu]<%s:%d>\x20" MSG "\n", pthread_self(), __func__, \
         __LINE__, ##__VA_ARGS__)

#endif

#define LOG_PRINTF_FATAL(MSG, ...) LOG_PRINTF(LOG_CRIT, MSG, ##__VA_ARGS__)
#define LOG_PRINTF_ERROR(MSG, ...) LOG_PRINTF(LOG_ERR, MSG, ##__VA_ARGS__)
#define LOG_PRINTF_WARN(MSG, ...) LOG_PRINTF(LOG_WARNING, MSG, ##__VA_ARGS__)
#define LOG_PRINTF_NOTICE(MSG, ...) LOG_PRINTF(LOG_NOTICE, MSG, ##__VA_ARGS__)
#define LOG_PRINTF_INFO(MSG, ...) LOG_PRINTF(LOG_INFO, MSG, ##__VA_ARGS__)
#define LOG_PRINTF_DEBUG(MSG, ...) LOG_PRINTF(LOG_DEBUG, MSG, ##__VA_ARGS__)

// 使用标准流输出日志
#define LOG_STREAM(LEVEL, MSG)                  \
  do {                                          \
    std::ostringstream oss;                     \
    oss << std::boolalpha << MSG;               \
    LOG_PRINTF(LEVEL, "%s", oss.str().c_str()); \
  } while (false)

#define LOG__FATAL(MSG) LOG_STREAM(LOG_CRIT, MSG)
#define LOG__ERROR(MSG) LOG_STREAM(LOG_ERR, MSG)
#define LOG__WARN(MSG) LOG_STREAM(LOG_WARNING, MSG)
#define LOG__NOTICE(MSG) LOG_STREAM(LOG_NOTICE, MSG)
#define LOG__INFO(MSG) LOG_STREAM(LOG_INFO, MSG)
#define LOG__DEBUG(MSG) LOG_STREAM(LOG_DEBUG, MSG)

#endif  // CORE_LOG_LOGGER_HPP_
