#include "CollisionTestSubsystem.h"

#include "Utils/UnusedMacro.h"
#include "Utils/format.h"

#include "Physics/CollisionComponent.h"

#include "Input/InputSystem.h"
#include "Imgui/imgui.h"

#include "Graphics/TransformComponent.h"
#include "Graphics/CameraComponent.h"

#include "glm/glm.hpp"
#include "Graphics/RenderStats.h"

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

		Graphics::RenderStats const renderStats = m_registry.GetConstContextComponent<Graphics::RenderStats const>();

		//Get position of mouse
		//Get position of render window
		//when mouse is within the bounds of the render window
		//calculate the mouse's position related to that top right position

		cachedViewPortSize = { renderStats.ViewPortWidth, renderStats.ViewPortHeight };
		cachedViewPortStart = { renderStats.ViewPortStartX, renderStats.ViewPortStartY };
		glm::dvec2 mousePosition = m_inputSystem.GetMousePosition();
		//glfwMousePos = mousePosition;

		ImVec2 const imVec2ImGuiMousePositon = ImGui::GetIO().MousePos;

		mousePosition.x = imVec2ImGuiMousePositon.x;
		mousePosition.y = imVec2ImGuiMousePositon.y;

		//Cache off the Mouse position
		imGuiMousePos = mousePosition;

		xAxis = mousePosition.x >= static_cast< double >( renderStats.ViewPortStartX )
			&& mousePosition.x <= static_cast< double >( renderStats.ViewPortStartX + renderStats.ViewPortWidth );

		yAxis = mousePosition.y >= static_cast< double >( renderStats.ViewPortStartY )
			&& mousePosition.y <= static_cast< double >( renderStats.ViewPortStartY + renderStats.ViewPortHeight );

		if ( xAxis && yAxis )
		{
			//Get the mouse position from the top left of the render window
			mousePosition -= cachedViewPortStart;

			//Scale the mouse position to a range of 0 - 2
			mousePosition /= cachedViewPortSize / 2;

			//Move it to the range of -1 to 1
			mousePosition -= glm::dvec2(1, 1);

			//Invert the Y so 1,1 is top right
			mousePosition.y = -mousePosition.y;

			//m_registry.ForConstView<Graphics::CameraComponent const, Graphics::ActiveCameraTag const>(
			//	[&mousePosition, &renderStats, this] ( Graphics::CameraComponent const & camera )
			//	{
			//		//glm::inverse( camera.m_viewMatrix * camera.m_projectionMatrix );
			//	} );

			cachedMappedMouseCoords = mousePosition;
			Graphics::TransformComponent & transform = m_registry.GetComponent<Graphics::TransformComponent>( m_firstEntity );
			transform.Position = glm::vec3( mousePosition, transform.Position.z );
		}
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

		if ( m_showDebugWindow )
		{
			if ( ImGui::Begin( "Debug Collision" ) )
			{
				ImGui::Text( "ViewPort Info" );
				ImGui::Text( fmt::format( "Viewport Start| x:{} y:{}", cachedViewPortStart.x, cachedViewPortStart.y ).c_str() );
				ImGui::Text( fmt::format( "Viewport End| x:{} y:{}", cachedViewPortStart.x + cachedViewPortSize.x, cachedViewPortStart.y + cachedViewPortSize.y ).c_str() );
				ImGui::Text( fmt::format( "Viewport Width| x:{}", cachedViewPortSize.x ).c_str() );
				ImGui::Text( fmt::format( "Viewport Height| y:{}", cachedViewPortSize.y ).c_str() );
				ImGui::Separator();
				ImGui::Text( "Mouse Coords" );
				
				//ImGui::Text( fmt::format( "GLFW | x:{} y:{}", glfwMousePos.x, glfwMousePos.y ).c_str() );
				ImGui::Text( fmt::format( "ImGui| x:{} y:{}", imGuiMousePos.x, imGuiMousePos.y ).c_str() );

				glm::dvec2 mouseminusviewport = imGuiMousePos - static_cast<glm::dvec2>(cachedViewPortStart);
				ImGui::Text( fmt::format("MousePosition - CachedViewPortStart|  x:{} y:{} ", mouseminusviewport.x, mouseminusviewport.y ).c_str() );

				glm::dvec2 MousePositionDividedByViewPortSize = mouseminusviewport / static_cast<glm::dvec2>( cachedViewPortSize / 2 );
				ImGui::Text( fmt::format("MousePosition / cachedViewPortSize| x:{} y:{}", MousePositionDividedByViewPortSize.x, MousePositionDividedByViewPortSize.y ).c_str() );

				glm::dvec2 MousePositionMinusOneOneVec = MousePositionDividedByViewPortSize - glm::dvec2( 1, 1 );
				ImGui::Text( fmt::format("MousePosition - 1,1 | x:{}, y:{}", MousePositionMinusOneOneVec.x, MousePositionMinusOneOneVec.y).c_str() );

				//Invert the Y so 1,1 is top right
				ImGui::Checkbox( "xAxis", &xAxis );
				ImGui::Checkbox( "yAxis", &yAxis );
				ImGui::Separator();
				ImGui::Text( "Modified Mouse Coords" );
				ImGui::Text( fmt::format( "Mapped| x:{} y:{}", cachedMappedMouseCoords.x, cachedMappedMouseCoords.y ).c_str() );
				//ImGui::Text( fmt::format( "Offset| x:{} y:{}", cachedMouseCoordsOffset.x, cachedMouseCoordsOffset.y ).c_str() );
			}
			ImGui::End();
		}
	}
}
