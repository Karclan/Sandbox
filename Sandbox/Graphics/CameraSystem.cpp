#include "CameraSystem.h"

#include "../Utils/Logger.h"

#include "CameraComponent.h"
#include "TransformComponent.h"

#include "imgui.h"
#include "../Utils/UnusedMacro.h"

namespace Graphics
{
	CameraSystem::CameraSystem( Core::Registry & registry )
		: Core::Subsystem( registry )
		, m_cameraInspector( registry )
	{

	}

	Core::Entity CreateCamera( std::string && CameraName, ProjectionType ProjType, Core::Registry & Registry )
	{
		auto entity = Registry.CreateEntity( std::move(CameraName) );

		Graphics::CameraComponent cameraComponent;
		cameraComponent.m_projectionType = ProjType;
		//cameraComponent.m_name = std::move( CameraName );
		Registry.AddComponent<Graphics::CameraComponent>( entity, cameraComponent );
		Registry.AddComponent<Graphics::TransformComponent>( entity, Graphics::TransformComponent {} );

		return entity;
	}

	Core::Entity CameraSystem::CreateOrthographicCamera( std::string && CameraName )
	{
		return CreateCamera( std::move( CameraName ), ProjectionType::Orthographic, m_registry );
	}

	Core::Entity CameraSystem::CreatePerspectiveCamera( std::string && CameraName )
	{
		return CreateCamera( std::move( CameraName ), ProjectionType::Perspective, m_registry );
	}

	void CameraSystem::SetActiveCamera( const std::string & CameraName )
	{
		Utils::Logger::LogMessage( Utils::LogType::Info, "Changed Camera\n" );

		m_registry.ForView<Graphics::CameraComponent const, Generic::NameComponent const>(
			[this, &CameraName] ( Core::Entity entity, CameraComponent const & cameraComponent, Generic::NameComponent const & nameComponent )
			{
				if ( m_registry.HasComponent<ActiveCameraTag>( entity ) )
				{
					m_registry.RemoveComponent<ActiveCameraTag>( entity );
				}

				if ( nameComponent.Name == CameraName )
				{
					m_registry.AddComponent<ActiveCameraTag>( entity );
				}

			} );
	}

	void CameraSystem::FixedUpdate( double deltaTime ) noexcept
	{
		UNUSED( deltaTime );

		m_registry.ForView<CameraComponent, TransformComponent>(
			[] ( CameraComponent & cameraComponent, TransformComponent & transformComponent )
			{
				if ( cameraComponent.m_dirtyProjectionMatrix )
				{
					float aspect = static_cast< float >( ( cameraComponent.m_viewPortWidth ) / ( cameraComponent.m_viewPortHeight > 0 ? cameraComponent.m_viewPortHeight : 1.f ) );

					switch ( cameraComponent.m_projectionType )
					{
						case ProjectionType::Orthographic:
						{
							cameraComponent.m_projectionMatrix = glm::ortho( cameraComponent.m_orthoLeft, cameraComponent.m_orthoRight, cameraComponent.m_orthoBottom, cameraComponent.m_orthoTop, cameraComponent.m_nearPlane, cameraComponent.m_farPlane );
							//cameraComponent.m_projectionMatrix = glm::ortho( 0.f, 800.f, 0.f, 600.f, -1.f, 1.f );
							break;
						}
						case ProjectionType::Perspective:
						{
							cameraComponent.m_projectionMatrix = glm::perspective( glm::radians( cameraComponent.m_fov ), aspect, cameraComponent.m_nearPlane, cameraComponent.m_farPlane );
							break;
						}
						default:
						{
							Utils::Logger::LogMessage( Utils::LogType::Warning, "Unhandled Camera Perspective type!\n" );
							break;
						}
					}
					
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

						switch ( cameraComponent.m_projectionType )
						{
							case ProjectionType::Orthographic:
							{
								//Note: current m_orbit isn't handled for transform. I imagine we'd rotate based off our up vector similar to perspective
								glm::vec3 eulers = glm::eulerAngles( transformComponent.RotationQuat );
								glm::mat4 transform = glm::translate( glm::mat4( 1.f ), transformComponent.Position ) * glm::rotate( glm::mat4( 1.f ), eulers.z, glm::vec3( 0.f, 0.f, 1.f ) );

								cameraComponent.m_viewMatrix = glm::inverse( transform );
								break;
							}
							case ProjectionType::Perspective:
							{
								glm::vec3 rotatedPosition = glm::vec4( transformComponent.Position, 1.f ) * rotationMatrix;
								cameraComponent.m_viewMatrix = glm::lookAt( rotatedPosition, cameraComponent.m_target, up );
								break;
							}
							default:
							{
								Utils::Logger::LogMessage( Utils::LogType::Warning, "Unhandled Camera Perspective type!\n" );
								break;
							}
						}

						
					}
					else
					{
						switch ( cameraComponent.m_projectionType )
						{
							case ProjectionType::Orthographic:
							{
								glm::vec3 eulers = glm::eulerAngles( transformComponent.RotationQuat );
								glm::mat4 transform = glm::translate( glm::mat4( 1.f ), transformComponent.Position ) * glm::rotate( glm::mat4( 1.f ), eulers.z, glm::vec3( 0.f, 0.f, 1.f ) );

								cameraComponent.m_viewMatrix = glm::inverse( transform );
								break;
							}
							case ProjectionType::Perspective:
							{
								cameraComponent.m_viewMatrix = glm::lookAt( transformComponent.Position, transformComponent.Position + forward, up );
								break;
							}
							default:
							{
								Utils::Logger::LogMessage( Utils::LogType::Warning, "Unhandled Camera Perspective type!\n" );
								break;
							}
						}
						//glm::vec3 rotatedPosition = glm::vec4( transformComponent.Position, 1.f ) * rotationMatrix;
						//cameraComponent.m_viewMatrix = glm::lookAt( rotatedPosition, cameraComponent.m_target, up );
						//Look at 0,0,0?
						//cameraComponent.m_viewMatrix = glm::lookAt( transformComponent.Position, glm::vec3{0.f}, up );
						//LookAt "forwards"
						
					}
				}
			} );

		//Camera Controls
	}

	void CameraSystem::Update( double deltaTime ) noexcept
	{
		UNUSED( deltaTime );
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