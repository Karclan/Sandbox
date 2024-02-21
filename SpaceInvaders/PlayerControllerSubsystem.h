#pragma once

#include "Core/Subsystem.h"
#include "glm/fwd.hpp"

namespace Input
{
	class InputSystem;
}

namespace Game
{
	class PlayerControllerSubsystem : public Core::Subsystem, public Core::ImGuiWindowBuilder
	{
	public:
		PlayerControllerSubsystem( Core::Registry & registry, Graphics::Window & windowRef, Input::InputSystem const & inputSystem );
		virtual ~PlayerControllerSubsystem() = default;

		void FixedUpdate( double deltaTime ) override;

		void Update( double deltaTime ) override;

		void BuildWindow( double deltaTime ) override;

	private:
		void CreateComponents( Graphics::Window & windowRef );

		Core::Entity m_PlayerEntity = Core::NullEntity;
		Input::InputSystem const & m_inputSystem;

		std::vector<glm::vec3> m_mousePositions;
	};
}
