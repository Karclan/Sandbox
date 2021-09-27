#pragma once

#include "../Core/Subsystem.h"

#include "imgui.h"
#include "Logger.h"

namespace Utils
{

	class LoggingWindow : public Core::ImGuiWindowBuilder
	{
	public:
		LoggingWindow( entt::registry& registry );
		~LoggingWindow();
		void BuildWindow( double deltaTime ) override;

		void AddLog( const char* fmt, ... ) IM_FMTARGS( 2 );
		void Clear();
	private:
		void LogMessageCallback( Utils::LogMessagePair const& arg );

		ImGuiTextBuffer Buf {};
		ImGuiTextFilter Filter {};
		ImVector<int> LineOffsets {}; // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool AutoScroll {false};	 // Keep scrolling if already at the bottom
		Utils::LogCallback::Handle LogHandle {};
	};
}

struct ExampleAppLog
{


};
