#include "CameraInspector.h"

#include "../Core/CoreRegistry.h"
#include "../Utils/Logger.h"

#include "CameraComponent.h"
#include "TransformComponent.h"

#include "imgui.h"

namespace Graphics
{

	CameraInspector::CameraInspector( Core::Registry& registry )
		: m_registry( registry )
	{
	}

	void CameraInspector::BuildWindow( double deltaTime )
	{
		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "Camera Inspector" ) )
			{
				ImGui::Checkbox( "View Cameras", &m_showCameraControls );
				ImGui::Checkbox( "Camera Controls", &m_showCameraControls );
				ImGui::Checkbox( "Camera Debug", &m_showCameraDebug );
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if ( m_showCameraControls )
		{
			BuildCameraControlsWindow();
		}
	}

	void CameraInspector::BuildCameraControlsWindow()
	{
		m_registry.ForView<CameraComponent, TransformComponent, ActiveCameraTag>( 
			[this] ( CameraComponent& cameraComponent, TransformComponent& transformComponent )
			{
				if ( ImGui::Begin( "Graphics" ) )
				{
					ImGui::Text( "Camera" );
					static float DragPower = 1.f;
					ImGui::SliderFloat( "Drag Power", &DragPower, 0.01f, 10.f );
					ImGui::DragFloat3( "Position", &transformComponent.Position.x, DragPower );
					ImGui::SameLine();
					if ( ImGui::Button( "Reset##Position" ) )
					{
						transformComponent.Position = glm::vec3( 0.f, 0.f, -10.f );
					}
					glm::vec4 rotatedPosition = transformComponent.RotationQuat * glm::vec4( transformComponent.Position, 1.f );
					ImGui::Text( "Rotated Position: %f, %f, %f", rotatedPosition.x, rotatedPosition.y, rotatedPosition.z );
					ImGui::Separator();

					ImGui::DragFloat3( "Target", &cameraComponent.m_target.x, DragPower );
					ImGui::SameLine();
					if ( ImGui::Button( "Reset##Target" ) )
					{
						cameraComponent.m_target = Constants::Forward;
					}

					ImGui::Separator();

					if ( !ImGui::IsAnyMouseDown() )
					{
						m_activeCameraEuler = glm::degrees( glm::eulerAngles( transformComponent.RotationQuat ) );
					}

					ImGui::Separator();
					ImGui::Text( "Global Rotation" );
					float old = m_activeCameraEuler.x;
					if ( ImGui::DragFloat( "Pitch", &m_activeCameraEuler.x, DragPower ) )
					{
						transformComponent.RotationQuat *= glm::angleAxis( glm::radians( old - m_activeCameraEuler.x ), Constants::Right );
					}
					old = m_activeCameraEuler.y;
					if ( ImGui::DragFloat( "Yaw", &m_activeCameraEuler.y, DragPower ) )
					{
						transformComponent.RotationQuat *= glm::angleAxis( glm::radians( old - m_activeCameraEuler.y ), Constants::Up );
					}
					old = m_activeCameraEuler.z;
					if ( ImGui::DragFloat( "Roll", &m_activeCameraEuler.z, DragPower ) )
					{
						transformComponent.RotationQuat *= glm::angleAxis( glm::radians( old - m_activeCameraEuler.z ), Constants::Forward );
					}

					ImGui::Separator();
					ImGui::Text( "Euler Angles" );
					if ( ImGui::DragFloat3( "Rotation", &m_activeCameraEuler.x, DragPower ) )
					{
						transformComponent.RotationQuat = glm::radians( m_activeCameraEuler );
					}
					ImGui::SameLine();
					if ( ImGui::Button( "Reset##Rot" ) )
					{
						transformComponent.RotationQuat = glm::quat_identity<float, glm::defaultp>();
					}
					ImGui::Separator();

					ImGui::Checkbox( "Orbit", &cameraComponent.m_orbit );

					ImGui::Checkbox( "Live Update", &m_liveCameraUpdate );
					ImGui::SameLine();
					if ( ImGui::Button( "Update" ) || m_liveCameraUpdate )
					{
						cameraComponent.m_dirtyProjectionMatrix = true;
						cameraComponent.m_dirtyViewMatrix = true;
					}

					ImGui::Separator();
				}
				ImGui::End();
			}
		);
	}

}
