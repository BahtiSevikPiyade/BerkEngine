#pragma once
#include "bepch.h"
#include "spdlog/spdlog.h"

namespace BerkEngine {
    class Log {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Motor için log makroları (Kullanımı kolay olsun diye)
#define BE_CORE_ERROR(...) ::BerkEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BE_CORE_WARN(...)  ::BerkEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BE_CORE_INFO(...)  ::BerkEngine::Log::GetCoreLogger()->info(__VA_ARGS__)

// Uygulama (Sandbox) için log makroları
#define BE_ERROR(...)      ::BerkEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define BE_INFO(...)       ::BerkEngine::Log::GetClientLogger()->info(__VA_ARGS__)

// Assert makrosu
#ifdef BE_DEBUG
    #define BE_ASSERT(x, msg) { if(!(x)) { BE_CORE_ERROR("ASSERT FAILED: {}", msg); __debugbreak(); } }
#else
    #define BE_ASSERT(x, msg)
#endif