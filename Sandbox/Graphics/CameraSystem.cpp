#include "CameraSystem.h"
#include "imgui.h"

namespace Graphics
{
	CameraSystem::CameraSystem( Core::Registry& registry )
		: Core::Subsystem( registry )
		, m_cameraInspector( registry )
	{

	}

	void CameraSystem::FixedUpdate( double deltaTime )
	{
		//Camera Controls
	}

	void CameraSystem::Update( double deltaTime )
	{
	
	}

	void CameraSystem::BuildWindow( double deltaTime )
	{
		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu("Cameras") )
			{
			}
			ImGui::EndMainMenuBar();
		}

		m_cameraInspector.BuildWindow( deltaTime );

	}

}