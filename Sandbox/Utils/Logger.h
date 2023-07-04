#pragma once

#include "format.h"

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "Delegate.h"

namespace Utils
{
	// Logging system
	// Used to output text with var args and formatting
	// Outputs to VS output and log file
	enum class LogType
	{
		Spam,
		Trace,
		Info,
		Warning,
		Error,
		Critical,

	};

	struct LogMessagePair
	{
		LogMessagePair( LogType inType, char const* inMessage ) noexcept
			: type( inType )
			, message( inMessage )
		{}

		LogType type;
		char const* message;
	};

	using LogCallback = Delegate<LogMessagePair const&>;

	class Logger
	{
	public:
		static void LogMessage( LogType const type, std::string const & message ) noexcept;
		static void LogMessage( LogType const type, char const* message ) noexcept;
		static void LogMessage( LogMessagePair Message );

		template<typename... Args>
		static void LogMessageTrace( const char * fmt, const Args &... args )
		{
			LogMessage( LogType::Trace, fmt::format( fmt, args... ).c_str() );
		}

		template<typename... Args>
		static void LogMessageInfo( const char * fmt, const Args &... args )
		{
			LogMessage( LogType::Info, fmt::format( fmt, args... ).c_str() );
		}

		template<typename... Args>
		static void LogMessageWarn( const char * fmt, const Args &... args )
		{
			LogMessage( LogType::Warning, fmt::format( fmt, args... ).c_str() );
		}

		template<typename... Args>
		static void LogMessageError( const char * fmt, const Args &... args )
		{
			LogMessage( LogType::Error, fmt::format( fmt, args... ).c_str() );
		}

		template<typename... Args>
		static void LogMessageCritical( const char * fmt, const Args &... args )
		{
			LogMessage( LogType::Critical, fmt::format( fmt, args... ).c_str() );
		}

		template<class Class>
		static LogCallback::Handle RegisterCallback( Class& obj, void ( Class::* func )( LogMessagePair const & ) )
		{
			Logger& instance = GetInstance();
			return instance.callbackDelegate.Connect( obj, func );
		}

		static void DeregisterCallback( LogCallback::Handle & handle )
		{
			Logger& instance = GetInstance();
			instance.callbackDelegate.Disconnect( handle );
		}

	private:
		static Logger& GetInstance();

		LogCallback callbackDelegate;
	};
}

// Core log macros
#define SB_CORE_TRACE(...)    ::Utils::Logger::LogMessageTrace(__VA_ARGS__)
#define SB_CORE_INFO(...)     ::Utils::Logger::LogMessageInfo(__VA_ARGS__)
#define SB_CORE_WARN(...)     ::Utils::Logger::LogMessageWarn(__VA_ARGS__)
#define SB_CORE_CRITICAL(...) ::Utils::Logger::LogMessageCritical(__VA_ARGS__)
#define SB_CORE_ERROR(...)    ::Utils::Logger::LogMessageError(__VA_ARGS__)

