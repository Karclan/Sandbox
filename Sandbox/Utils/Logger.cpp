#include "Logger.h"


#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#define LEAN_AND_MEAN
#include <Windows.h>
#include <debugapi.h>

namespace Utils
{
	void Logger::LogMessage(LogType const type, std::string const& message) noexcept
	{
		LogMessage( LogMessagePair( type, message.c_str() ) );
	}
	void Logger::LogMessage( LogType const type, char const* message ) noexcept
	{
		LogMessage( LogMessagePair( type, message ) );
	}

	void Logger::LogMessage( LogMessagePair message )
	{
		auto const now = std::chrono::system_clock::now();

		std::wostringstream OS;
		switch ( message.type )
		{
			case LogType::Spam:
				OS << "Spam - ";
				break;
			case LogType::Trace:
				OS << "Trace - ";
				break;
			case LogType::Info:
				OS << "Info - ";
				break;
			case LogType::Warning:
				OS << "Warning - ";
				break;
			case LogType::Error:
				OS << "Error - ";
				break;
			case LogType::Critical:
				OS << "Critical - ";
				break;
		}
		OS << message.message;
		OutputDebugString( OS.str().c_str() );

		Logger const & logger = GetInstance();
		logger.callbackDelegate( message );
	}

	Logger& Logger::GetInstance()
	{
		static Logger inst = Logger();
		return inst;
	}


}
