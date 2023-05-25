#pragma once

#include "Core/Subsystem.h"

#include "Core/CoreRegistry.h"

namespace Graphics
{
	class Window;
	class CameraSystem;
}

namespace Game
{
	class Basic2DGameTestSubsystem : public Core::Subsystem
	{
	public:

		Basic2DGameTestSubsystem( Core::Registry & registryRef, Graphics::Window & window, Graphics::CameraSystem & cameraSystem );
		void Init();
		void FixedUpdate( double deltaTime ) override;

		void Update( double deltaTime ) override;
		
	private:

		Graphics::Window & m_windowRef;
		Graphics::CameraSystem & m_cameraSystemRef;
	};
}