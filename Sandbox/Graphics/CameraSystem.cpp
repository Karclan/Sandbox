#include "CameraSystem.h"

#include "../Utils/Logger.h"

#include "CameraComponent.h"
#include "TransformComponent.h"

#include "imgui.h"

namespace Graphics
{
	CameraSystem::CameraSystem( Core::Registry& registry )
		: Core::Subsystem( registry )
		, m_cameraInspector( registry )
	{

	}

	void CameraSystem::CreateCamera( const std::string&& CameraName )
	{
		auto entity = m_registry.CreateEntity();

		Graphics::CameraComponent cameraComponent;
		cameraComponent.m_name = std::move( CameraName );
		m_registry.AddComponent<Graphics::CameraComponent>( entity, cameraComponent );
		m_registry.AddComponent<Graphics::TransformComponent>( entity, Graphics::TransformComponent {} );
	}

	void CameraSystem::SetActiveCamera( const std::string& CameraName )
	{
		Utils::Logger::LogMessage( Utils::LogType::Info, "Changed Camera\n" );

		m_registry.ForView<Graphics::CameraComponent>(
			[this, &CameraName] ( Core::Entity entity, CameraComponent& cameraComponent )
			{
				if ( m_registry.HasComponent<ActiveCameraTag>( entity ) )
				{
					m_registry.RemoveComponent<ActiveCameraTag>( entity );
				}

				if ( cameraComponent.m_name == CameraName )
				{
					m_registry.AddComponent<ActiveCameraTag>( entity );
				}

			} );
	}

	void CameraSystem::FixedUpdate( double deltaTime )
	{
		m_registry.ForView<CameraComponent, TransformComponent>(
			[] ( CameraComponent& cameraComponent, TransformComponent& transformComponent )
			{
				if ( cameraComponent.m_dirtyProjectionMatrix )
				{
					float aspect = static_cast< float >( ( cameraComponent.m_viewPortWidth ) / ( cameraComponent.m_viewPortHeight > 0 ? cameraComponent.m_viewPortHeight : 1.f ) );

					cameraComponent.m_projectionMatrix = glm::perspective( glm::radians( cameraComponent.m_fov ), aspect, cameraComponent.m_nearPlane, cameraComponent.m_farPlane );
				}

				if ( cameraComponent.m_dirtyViewMatrix )
				{
					glm::mat4 rotationMatrix = glm::toMat4( transformComponent.RotationQuat );

					glm::vec3 const forward = glm::normalize( rotationMatrix * glm::vec4( Constants::Forward, 1.f ) );
					glm::vec3 const right = glm::normalize( rotationMatrix * glm::vec4( Constants::Right, 1.f ) );
					glm::vec3 const up = glm::normalize( rotationMatrix * glm::vec4( Constants::Up, 1.f ) );

					if ( cameraComponent.m_orbit )
					{
						//glm::mat4 translation = glm::translate( glm::mat4( 1.0f ), transformComponent.Position );
						//glm::mat4 rotationMatrix = glm::toMat4( transformComponent.RotationQuat );
						//glm::mat4 scale = glm::scale( glm::mat4( 1.f ), transformComponent.Scale );
						//cameraComponent.m_viewMatrix = translation * rotationMatrix * scale;

						glm::vec3 rotatedPosition = glm::vec4( transformComponent.Position, 1.f ) * rotationMatrix;
						cameraComponent.m_viewMatrix = glm::lookAt( rotatedPosition, cameraComponent.m_target, up );
					}
					else
					{
						//glm::vec3 rotatedPosition = glm::vec4( transformComponent.Position, 1.f ) * rotationMatrix;
						//cameraComponent.m_viewMatrix = glm::lookAt( rotatedPosition, cameraComponent.m_target, up );
						//Look at 0,0,0?
						//cameraComponent.m_viewMatrix = glm::lookAt( transformComponent.Position, glm::vec3{0.f}, up );
						//LookAt "forwards"
						cameraComponent.m_viewMatrix = glm::lookAt( transformComponent.Position, transformComponent.Position + forward, up );
					}
				}
			} );

		//Camera Controls
	}

	void CameraSystem::Update( double deltaTime )
	{

	}

	void CameraSystem::BuildWindow( double deltaTime )
	{
		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "Cameras" ) )
			{
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		m_cameraInspector.BuildWindow( deltaTime );

	}

}