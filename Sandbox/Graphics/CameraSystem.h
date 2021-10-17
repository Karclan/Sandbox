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

		void CreateCamera( std::string&& CameraName );
		void SetActiveCamera( const std::string& CameraName );

		void FixedUpdate( double deltaTime ) noexcept override;
		void Update( double deltaTime ) noexcept override;
		void BuildWindow( double deltaTime ) override;
	private:
		CameraInspector m_cameraInspector;
	};
}

