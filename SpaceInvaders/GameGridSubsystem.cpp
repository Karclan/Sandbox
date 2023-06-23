#include "GameGridSubsystem.h"

#include "Utils/UnusedMacro.h"
#include "Graphics/Renderer.h"

#include "GameGridComponent.h"

#include "Graphics/Window.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/GraphicsDraw.h"

namespace Game
{

	GameGridSubsystem::GameGridSubsystem( Core::Registry & registry, Graphics::Window & window )
		: Core::Subsystem( registry )
		, m_Window( window )
	{
		CreateComponents();
	}

	void GameGridSubsystem::CreateComponents()
	{
		//Construction of our grid entity, the player and enemies will refer to this for their movement behaviour
		Core::Entity GridEntity = m_registry.CreateEntity( "Grid" );
		m_registry.AddComponent<GameGridComponent>( GridEntity );
		m_registry.AddComponent<Graphics::TransformComponent>( GridEntity );
	}

	void GameGridSubsystem::FixedUpdate( double deltaTime )
	{
		UNUSED( deltaTime );
	}

	void GameGridSubsystem::Update( double deltaTime )
	{
		UNUSED( deltaTime );
		m_registry.ForView<GameGridComponent, Graphics::TransformComponent>( [this] ( GameGridComponent & gridComponent, Graphics::TransformComponent & transform )
			{
				if ( gridComponent.DimensionsChanged )
				{
					int const halfHeight = gridComponent.Height / 2;
					int const halfWidth = gridComponent.Width / 2;

					gridComponent.TopLeft = { -( transform.Position.x + halfWidth ), transform.Position.y + halfHeight, transform.Position.z };
					gridComponent.BottomRight = { transform.Position.x + halfWidth, -( transform.Position.y + halfHeight ), transform.Position.z };

					gridComponent.DimensionsChanged = false;
				}

				if ( gridComponent.Visualize )
				{
					glm::vec3 const topLeft = glm::vec3( gridComponent.TopLeft.x, gridComponent.TopLeft.y, transform.Position.z );
					glm::vec3 const topRight = glm::vec3( gridComponent.TopLeft.x + gridComponent.Width, gridComponent.TopLeft.y, transform.Position.z );
					glm::vec3 const bottomLeft = glm::vec3( gridComponent.BottomRight.x - gridComponent.Width, gridComponent.BottomRight.y, transform.Position.z );
					glm::vec3 const bottomRight = glm::vec3( gridComponent.BottomRight.x, gridComponent.BottomRight.y, transform.Position.z );
					Graphics::GraphicsDraw::DrawLine( topLeft, topRight, gridComponent.DebugColour, 1.f );
					Graphics::GraphicsDraw::DrawLine( topLeft, bottomLeft, gridComponent.DebugColour, 1.f );
					Graphics::GraphicsDraw::DrawLine( topRight, bottomRight, gridComponent.DebugColour, 1.f );
					Graphics::GraphicsDraw::DrawLine( bottomRight, bottomLeft, gridComponent.DebugColour, 1.f );
				}
			} );
	}
}
