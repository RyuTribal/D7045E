#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)


namespace D7045E
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}

// Core log macros
#define CORE_FATAL(...) ::D7045E::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CORE_ERROR(...) ::D7045E::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_WARN(...) ::D7045E::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_INFO(...) ::D7045E::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_TRACE(...) ::D7045E::Log::GetCoreLogger()->trace(__VA_ARGS__)
