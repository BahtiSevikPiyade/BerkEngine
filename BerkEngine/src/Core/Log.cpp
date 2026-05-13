#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace BerkEngine {
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init() {
        spdlog::set_pattern("%^[%T] %n: %v%$"); // Zaman, isim ve mesaj formatı

        // Console sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::trace);

        // File sink
        try {
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("berkengine.log", true);
            file_sink->set_level(spdlog::level::trace);

            // Combine sinks
            std::vector<spdlog::sink_ptr> sinks;
            sinks.push_back(console_sink);
            sinks.push_back(file_sink);

            s_CoreLogger = std::make_shared<spdlog::logger>("BERK_ENGINE", sinks.begin(), sinks.end());
            s_CoreLogger->set_level(spdlog::level::trace);
            s_CoreLogger->flush_on(spdlog::level::trace);  // Flush her seviyede

            s_ClientLogger = std::make_shared<spdlog::logger>("APP", sinks.begin(), sinks.end());
            s_ClientLogger->set_level(spdlog::level::trace);
            s_ClientLogger->flush_on(spdlog::level::trace);
        } catch (const spdlog::spdlog_ex& ex) {
            // Fallback to console nur
            s_CoreLogger = std::make_shared<spdlog::logger>("BERK_ENGINE", console_sink);
            s_CoreLogger->set_level(spdlog::level::trace);
            s_CoreLogger->flush_on(spdlog::level::trace);

            s_ClientLogger = std::make_shared<spdlog::logger>("APP", console_sink);
            s_ClientLogger->set_level(spdlog::level::trace);
            s_ClientLogger->flush_on(spdlog::level::trace);
        }
    }
}