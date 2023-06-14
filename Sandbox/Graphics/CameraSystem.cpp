#include "CameraSystem.h"

#include "../Utils/Logger.h"
#include "../Utils/UnusedMacro.h"

#include "CameraComponent.h"
#include "CameraStatsComponent.h"
#include "TransformComponent.h"
#include "GraphicsTags.h"

#include "imgui.h"

#include <numeric>

namespace Graphics
{
	CameraSystem::CameraSystem( Core::Registry & registry )
		: Core::Subsystem( registry )
		, m_cameraInspector( registry )
	{
		m_registry.ctx().emplace<CameraStats>();
	}

	Core::Entity CreateCamera( std::string && CameraName, ProjectionType ProjType, Core::Registry & Registry )
	{
		auto entity = Registry.CreateEntity( std::move(CameraName) );

		Graphics::CameraComponent cameraComponent;
		cameraComponent.m_projectionType = ProjType;
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

		Core::Entity newMainCamera = Core::NullEntity;

		m_registry.ForView<CameraComponent const, Generic::NameComponent const>(
			[this, &CameraName, &newMainCamera] ( Core::Entity entity, CameraComponent const & cameraComponent, Generic::NameComponent const & nameComponent )
			{
				if ( m_registry.HasComponent<ActiveCameraTag>( entity ) )
				{
					m_registry.RemoveComponent<ActiveCameraTag>( entity );
				}

				if ( nameComponent.Name == CameraName )
				{
					m_registry.AddComponent<ActiveCameraTag>( entity );
					newMainCamera = entity;
				}

			} );

		CameraStats & stats = m_registry.ctx().get<CameraStats>();
		stats.m_mainCameraEntity = newMainCamera;
	}

	void CameraSystem::FixedUpdate( double deltaTime ) noexcept
	{
		UNUSED( deltaTime );

		InternalCameraUpdate( m_registry );

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

	void CameraSystem::InternalCameraUpdate( Core::Registry & registry )
	{
		registry.ForView<CameraComponent, TransformComponent>(
			[] ( CameraComponent & cameraComponent, TransformComponent & transformComponent )
			{
				if ( cameraComponent.m_dirtyProjectionMatrix )
				{
					float aspect = static_cast< float >( ( cameraComponent.m_viewPortWidth ) / ( cameraComponent.m_viewPortHeight > 0 ? cameraComponent.m_viewPortHeight : 1.f ) );
					switch ( cameraComponent.m_projectionType )
					{
						case ProjectionType::Orthographic:
						{
							//The commented out sections of this are related to having the perspective camera and orthographic camera "look" the same when switching between them. 
							//However this code should be shifted to an editor only camera and that sort of behaviour shouldn't exist on all orthographic projections as it's not a true ortho proj behaviour
							float ratio_size_per_depth = cameraComponent.m_orthoAspectScale;// glm::radians( cameraComponent.m_fov );
							//float distance = glm::length( cameraComponent.m_target - transformComponent.Position );
							float size_y = ratio_size_per_depth/* * distance*/;
							float size_x = ratio_size_per_depth /* distance*/ * aspect;
							cameraComponent.m_projectionMatrix = glm::ortho( -size_x, size_x, -size_y, size_y, cameraComponent.m_nearPlane /** distance*/, cameraComponent.m_farPlane /** distance*/ );
							//cameraComponent.m_projectionMatrix = glm::scale( cameraComponent.m_projectionMatrix, glm::vec3( cameraComponent.m_orthoAspectScale, cameraComponent.m_orthoAspectScale, 1.f ) );

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
						switch ( cameraComponent.m_projectionType )
						{
							case ProjectionType::Orthographic:
							{
								//Note: current m_orbit isn't handled for transform. I imagine we'd rotate based off our up vector similar to perspective
								//glm::vec3 eulers = glm::eulerAngles( transformComponent.RotationQuat );
								//glm::mat4 transform = glm::translate( glm::mat4( 1.f ), transformComponent.Position ) * glm::rotate( glm::mat4( 1.f ), eulers.z, glm::vec3( 0.f, 0.f, 1.f ) );

								//cameraComponent.m_viewMatrix = glm::inverse( transform );

								glm::vec3 rotatedPosition = glm::vec4( transformComponent.Position, 1.f ) * rotationMatrix;
								cameraComponent.m_viewMatrix = glm::lookAt( rotatedPosition, cameraComponent.m_target, up );
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
								//glm::vec3 eulers = glm::eulerAngles( transformComponent.RotationQuat );
								//glm::mat4 transform = glm::translate( glm::mat4( 1.f ), transformComponent.Position ) * glm::rotate( glm::mat4( 1.f ), eulers.z, glm::vec3( 0.f, 0.f, 1.f ) );

								//cameraComponent.m_viewMatrix = glm::inverse( transform );

								cameraComponent.m_viewMatrix = glm::lookAt( transformComponent.Position, transformComponent.Position + forward, up );
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
					}
				}

			} );
	}
}
