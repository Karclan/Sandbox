#pragma once

#include "glm/glm.hpp"

namespace Core
{
	class Registry;
}

namespace Graphics
{
	class CameraInspector
	{
	public:
		CameraInspector( Core::Registry& registry );
		~CameraInspector() = default;

		void BuildWindow( double deltaTime );

	private:
		void BuildCameraControlsWindow();

		Core::Registry& m_registry;

		glm::vec3 m_activeCameraEuler { 0.f };
		bool m_showCameraControls { true };
		bool m_showCameraDebug { false };
		bool m_liveCameraUpdate { true };
	};
}