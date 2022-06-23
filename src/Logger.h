#ifndef LOGGER_H
#define LOGGER_H

#ifndef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <memory>
#include "Macros.h"
#include "spdlog/spdlog.h"
// this space is deliberately here because of the spdlog order. Do not remove
#include "spdlog/sinks/stdout_color_sinks.h"

class Logger {
    using consolLogger = spdlog::sinks::stdout_color_sink_st;

   public:
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    bool Check_GL_ErrorCode();

    spdlog::logger& GetLogger() {
        return m_logger;
    }

   private:
    Logger() {
#ifndef NDEBUG
        m_logger.set_level(spdlog::level::trace);
#else
        m_logger.set_level(spdlog::level::info);
#endif
    }
    spdlog::logger m_logger = spdlog::logger("Console", std::make_shared<consolLogger>());
};

// Checks if a debugger is attached
[[maybe_unused]] static bool CheckForDebugger(void) {
    char buf[256];
    int fd = open("/proc/self/status", O_RDONLY);
    if (fd == -1) {
        return false;
    }
    const int len = read(fd, buf, sizeof(buf));
    bool rc = false;
    if (len > 0) {
        const char* const debuggerPid = "TracerPid:\t";
        buf[len - 1] = '\0';
        const char* p = strstr(buf, debuggerPid);
        if (p != NULL) {
            rc = (strncmp(p + strlen(debuggerPid), "0\n", 2) != 0);
        }
    }
    close(fd);
    return rc;
}

#ifndef NDEBUG
#include <csignal>
// This macro
// Warning: Condition is only evaluated just once! Therefor bool evalCondition = !(cond)
#define ASSERT(cond)                                                                                                  \
    {                                                                                                                 \
        bool evalCondition = !(cond);                                                                                 \
        if (evalCondition && CheckForDebugger()) {                                                                    \
            Logger::GetInstance().GetLogger().warn("ASSERTION FAILED: {}, {}, {}", __FUNCTION__, __FILE__, __LINE__); \
            raise(SIGTRAP);                                                                                           \
        } else if (evalCondition) {                                                                                   \
            Logger::GetInstance().GetLogger().warn("ASSERTION FAILED: {}, {}, {}", __FUNCTION__, __FILE__, __LINE__); \
        }                                                                                                             \
    }
#else
#define ASSERT(cond)                                                                                              \
    if (!(cond)) {                                                                                                \
        Logger::GetInstance().GetLogger().warn("ASSERTION FAILED: {}, {}, {}", __FUNCTION__, __FILE__, __LINE__); \
    }
#endif

#define CHECK_GL_ERROR_(expr)                              \
    {                                                      \
        expr;                                              \
        ASSERT(Logger::GetInstance().Check_GL_ErrorCode()) \
    }

#define CHECK_GL_ERROR_WITH_OUTPUT_(outvar, expr)          \
    {                                                      \
        outvar = expr;                                     \
        ASSERT(Logger::GetInstance().Check_GL_ErrorCode()) \
    }

#endif
