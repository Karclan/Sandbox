#pragma once

#include "Core/Subsystem.h"

namespace Game
{
	class PlayerControllerSubsystem : public Core::Subsystem, Core::ImGuiWindowBuilder
	{
	public:
		PlayerControllerSubsystem( Core::Registry & registry, Graphics::Window & windowRef );
		virtual ~PlayerControllerSubsystem() = default;

		void FixedUpdate( double deltaTime ) override;

		void Update( double deltaTime ) override;

		void BuildWindow( double deltaTime ) override;

	private:
		void CreateComponents( Graphics::Window & windowRef );

		Core::Entity m_PlayerEntity = Core::NullEntity;
	};
}
