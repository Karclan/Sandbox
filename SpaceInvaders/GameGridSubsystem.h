#pragma once
#include "Core/Subsystem.h"

namespace Graphics
{
	class Window;
}

namespace Game
{
	class GameGridSubsystem : public Core::Subsystem
	{
	public:
		GameGridSubsystem( Core::Registry & registry, Graphics::Window & renderer );

		void FixedUpdate( double deltaTime ) override;

		void Update( double deltaTime ) override;

	private:
		void CreateComponents();

		Graphics::Window & m_Window;
	};
}

