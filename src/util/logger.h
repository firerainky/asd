//
// Created by 赵启明 on 2023/8/4.
//

#ifndef ZJ_FHE_LIB_LOGGER_H
#define ZJ_FHE_LIB_LOGGER_H
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#define CONSOLE_LOG
#define LOG_FILE_NAME "zj_fhe_log.log"
#define LOG_FILE_PATH "logs/"
#define CONSOLE_LOG_LEVEL spdlog::level::debug
#define FILE_LOG_LEVEL spdlog::level::info
#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
namespace zhejiangfhe {
    class Logger {

    public:


        static Logger& GetInstance() {
            static Logger m_instance;
            return m_instance;
        }

        auto GetLogger() { return zj_logger; }




        ~Logger() {
            spdlog::drop_all();
        }
    private:
        std::shared_ptr<spdlog::logger> zj_logger;


        void Init() {
            std::vector<spdlog::sink_ptr> sinkList;

            std::string pattern = "[%Y-%m-%d %H:%M:%S.%e] [Process:%P] [Thread:%t] [%^%l%$] <%s>|<%#>|<%!> %v";

#ifdef CONSOLE_LOG
            auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            consoleSink->set_level(CONSOLE_LOG_LEVEL);
            consoleSink->set_pattern(pattern);
            sinkList.push_back(consoleSink);
#endif

            const std::string filename = LOG_FILE_NAME;
            const std::string filePath = LOG_FILE_PATH;
            const std::string fileFullPath = filePath + filename;
            auto fileSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(fileFullPath, 1, 0);
            fileSink->set_level(FILE_LOG_LEVEL);
            fileSink->set_pattern(pattern);
            sinkList.push_back(fileSink);


            zj_logger = std::make_shared<spdlog::logger>("zj_logger", begin(sinkList), end(sinkList));
            zj_logger->flush_on(spdlog::level::info); // 设置当触发 info 或更严重的错误时立刻刷新日志到 disk
            zj_logger->set_level(spdlog::level::trace);

            spdlog::register_logger(zj_logger); //register it if you need to access it globally
            spdlog::flush_on(spdlog::level::info); // 每隔10秒刷新一次日志
            spdlog::flush_every(std::chrono::seconds(1)); // 每隔10秒刷新一次日志

        }
        Logger() {
            Init();
        };
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

    };


#define BASE_LOGGER_CALL(logger, level, ...)                                                                                         \
            (logger)->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

#define ZJTrace(...) BASE_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::trace, __VA_ARGS__)
#define ZJDebug(...) BASE_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::debug, __VA_ARGS__)
#define ZJInfo(...) BASE_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::info, __VA_ARGS__)
#define ZJWarn(...) BASE_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::warn, __VA_ARGS__)
#define ZJError(...) BASE_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::err, __VA_ARGS__)
#define ZJCritical(...) BASE_LOGGER_CALL(Logger::GetInstance().GetLogger(), spdlog::level::critical, __VA_ARGS__)

}

#endif//ZJ_FHE_LIB_LOGGER_H
