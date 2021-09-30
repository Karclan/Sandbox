#pragma once

#include "../Core/Subsystem.h"

#include "CameraInspector.h"

namespace Graphics
{
	class CameraSystem : public Core::Subsystem, public Core::ImGuiWindowBuilder
	{
	public:
		CameraSystem( Core::Registry& registry );
		~CameraSystem() = default;

		void FixedUpdate( double deltaTime ) override;
		void Update( double deltaTime ) override;
		void BuildWindow( double deltaTime ) override;
	private:

		CameraInspector m_cameraInspector;
	};
}

