#pragma once

#include "../Core/Subsystem.h"

#include <map>

namespace Input
{
	class ActionId;

	class InputSystem : public Core::Subsystem, public Core::ImGuiWindowBuilder
	{
	public:
		InputSystem( Core::Registry& registry );
		~InputSystem() = default;

		bool TestAction( ActionId const& action ) const;

		void BuildWindow( double deltaTime ) override;

	private:

	};
}
