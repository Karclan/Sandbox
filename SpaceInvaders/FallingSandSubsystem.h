#pragma once

#include "Core/Subsystem.h"
#include "Input/InputSystem.h"

namespace Game
{
	class FallingSandSubsystem : public Core::Subsystem
	{
	public:
		FallingSandSubsystem( Core::Registry & registry, Input::InputSystem & inputSystem );
		~FallingSandSubsystem() = default;

		void InitComponents();

		void FixedUpdate( double deltaTime ) override;
		void Update( double deltaTime ) override;

	private:
		Core::Entity StateEntity = Core::NullEntity;

		Input::InputSystem & m_inputSystem;
	};
}

