#pragma once

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
		Info,
		Warning,
		Error,
		Critical,

	};

	struct LogMessagePair
	{
		LogMessagePair( LogType inType, char const* inMessage )
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
		static void LogMessage( LogType const type, char const* message );
		static void LogMessage( LogMessagePair Message );

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
