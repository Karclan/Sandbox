#pragma once

#include "InputState.h"

namespace Input
{
	
	//InputContext - A way of grouping various input actions into a stackable block that can be updated and queried
	//Example -> Startup -> Menu -> Gameplay -> Pause
	//Each of those are input context's which effectively gate keep any input actions that aren't valid right now
	class InputContext
	{
	public:

	private:
		ActionIdInputState ActionIdStateMap;

	};
}
