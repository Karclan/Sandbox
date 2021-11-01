#include "CollisionTestSubsystem.h"

#include "Utils/UnusedMacro.h"
#include "Utils/format.h"

#include "Physics/CollisionComponent.h"

#include "Input/InputSystem.h"
#include "Imgui/imgui.h"

#include "Graphics/TransformComponent.h"
#include "Graphics/CameraComponent.h"

#include "glm/glm.hpp"

namespace Game
{
	CollisionTestSubsystem::CollisionTestSubsystem( Core::Registry & registry, Input::InputSystem const & inputSystem )
		: Core::Subsystem( registry )
		, m_inputSystem( inputSystem )
	{
		m_firstEntity = m_registry.CreateEntity();
		m_secondEntity = m_registry.CreateEntity();
		auto const attachComponents = [this] ( Core::Entity entity, glm::vec3 position, float width, float height )
		{
			m_registry.AddComponent<Graphics::TransformComponent>( entity, position );
			m_registry.AddComponent<Physics::CollisionComponent>( entity );
			auto & extents = m_registry.AddComponent<Physics::SquareExtents>( entity );
			extents.width = width;
			extents.height = height;
		};

		attachComponents( m_firstEntity, { 2.f,2.f,0.f }, 1.f, 1.f );
		attachComponents( m_secondEntity, { 0.f, 0.f, 0.f }, 1.f, 1.f );
	}

	void CollisionTestSubsystem::FixedUpdate( double deltaTime )
	{
		UNUSED( deltaTime );
		glm::dvec2 mousePosition = m_inputSystem.GetMousePosition();
		cachedMouseCoords = mousePosition;
		m_registry.ForConstView<Graphics::CameraComponent const, Graphics::ActiveCameraTag const>(
			[&mousePosition] ( Graphics::CameraComponent const & camera )
			{
				//glm::inverse( camera.m_viewMatrix * camera.m_projectionMatrix );

				mousePosition.x /= camera.m_viewPortWidth / 2.f;
				mousePosition.y /= camera.m_viewPortHeight / 2.f;
				/*camera.m_viewPortHeight;
				camera.m_viewPortWidth;*/
			} );
		cachedMappedMouseCoords = mousePosition;
		Graphics::TransformComponent & transform = m_registry.GetComponent<Graphics::TransformComponent>( m_firstEntity );
		transform.Position = glm::vec3( mousePosition, transform.Position.z );
	}

	void CollisionTestSubsystem::Update( double deltaTime )
	{
		UNUSED( deltaTime );
	}

	void CollisionTestSubsystem::BuildWindow( double deltaTime )
	{
		UNUSED( deltaTime );

		if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "Collision" ) )
			{
				ImGui::Checkbox( "Debug", &m_showDebugWindow );
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if( ImGui::Begin( "Debug Collision", &m_showDebugWindow ) )
		{
			ImGui::Text( fmt::format( "MouseCoords - Screenspace| x:{} y:{}", cachedMouseCoords.x, cachedMouseCoords.y ).c_str() );
			ImGui::Text( fmt::format( "MouseCoords - \"Mapped\"| x:{} y:{}", cachedMappedMouseCoords.x, cachedMappedMouseCoords.y ).c_str() );
			ImGui::End();
		}
	}
}
