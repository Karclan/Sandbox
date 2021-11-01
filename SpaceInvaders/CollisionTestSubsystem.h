#pragma once
#include "Core/Subsystem.h"

#include <glm/glm.hpp>

namespace Input
{
	class InputSystem;
}

namespace Game
{

	class CollisionTestSubsystem final : public Core::Subsystem, public Core::ImGuiWindowBuilder
	{
	public:
		CollisionTestSubsystem( Core::Registry & registry, Input::InputSystem const & inputSystem );

		void FixedUpdate( double deltaTime ) override;

		void Update( double deltaTime ) override;

		void BuildWindow( double deltaTime ) override;

	private:
		bool m_showDebugWindow { false };
		Core::Entity m_firstEntity { Core::NullEntity };
		Core::Entity m_secondEntity { Core::NullEntity };

		Input::InputSystem const & m_inputSystem;

		glm::dvec2 cachedMouseCoords { 0.f };
		glm::dvec2 cachedMappedMouseCoords { 0.f };
	};
}

