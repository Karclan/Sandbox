#pragma once

#include "../Core/Subsystem.h"

#include "CameraInspector.h"

namespace Graphics
{

	class CameraSystem : public Core::Subsystem, public Core::ImGuiWindowBuilder
	{
		friend class Renderer;
	public:
		CameraSystem( Core::Registry & registry );
		~CameraSystem() = default;

		Core::Entity CreateOrthographicCamera( std::string && CameraName );
		Core::Entity CreatePerspectiveCamera( std::string && CameraName );
		void SetActiveCamera( const std::string & CameraName );

		void FixedUpdate( double deltaTime ) noexcept override;
		void Update( double deltaTime ) noexcept override;
		void BuildWindow( double deltaTime ) override;
	private:
		static void InternalCameraUpdate( Core::Registry & registry );

		CameraInspector m_cameraInspector;
	};
}

