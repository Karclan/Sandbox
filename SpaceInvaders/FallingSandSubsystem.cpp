#include "FallingSandSubsystem.h"

#include "Utils/UnusedMacro.h"
#include "FallingSandSimulationComponent.h"
#include "Graphics/GraphicsDraw.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/common.hpp"
#include "GameInputActions.h"

namespace Game
{
	/*TODO: Falling Sand Todo List
	* Draw the grid we can place sand in
	* Get cell position based off mouse position within the grid
	* Place sand
	* Have sand fall down
	* Resolve sand positions stacking on top of each other -> if cell below, left, right empty
	*/


	//Points seem to draw at size which is fixed based on something unrelated to camera position to z=0 where grid lays


	FallingSandSubsystem::FallingSandSubsystem( Core::Registry & registry, Input::InputSystem & inputSystem )
		: Core::Subsystem( registry )
		, m_inputSystem( inputSystem )
	{
		InitComponents();
	}

	void FallingSandSubsystem::InitComponents()
	{
		StateEntity = m_registry.CreateEntity();
		Game::FallingSandStateComponent & StateComponent = m_registry.AddComponent<Game::FallingSandStateComponent>( StateEntity );
		for ( unsigned int i = 0; i < StateComponent.GridSize; ++i )
		{
			for ( unsigned int j = 0; j < StateComponent.GridSize; ++j )
			{

				StateComponent.GridSandStateArray[i][j] = 0;
			}
		}

		//Test sand!
		StateComponent.GridSandStateArray[0][0] = 1;
		StateComponent.GridSandStateArray[1][1] = 76;

		StateComponent.GridSandStateArray[15][10] = 50;

		StateComponent.GridSandStateArray[29][10] = 120;
		StateComponent.GridSandStateArray[29][29] = 280;

		StateComponent.Gravity = glm::vec2( 0.f, 1.f );
	}

	void FallingSandSubsystem::FixedUpdate( double deltaTime )
	{
		UNUSED( deltaTime );
	}

	void FallingSandSubsystem::Update( double deltaTime )
	{
		UNUSED( deltaTime );

		auto & StateComponent = m_registry.GetComponent<Game::FallingSandStateComponent>( StateEntity );

		unsigned int const & CellSize = StateComponent.CellSize;
		float const halfCellSize = static_cast<float>(CellSize) / 2.f;
		glm::ivec2 & GridStartPosition = StateComponent.GridStartPosition;
		glm::ivec2 const GridEndPosition = { GridStartPosition.x + ( CellSize * StateComponent.GridSize ), GridStartPosition.y + ( CellSize * StateComponent.GridSize ) };

		//Draw the grid
		{
			for ( int i = 0; i <= StateComponent.GridSize; ++i )
			{
				//Horizontal Lines
				glm::vec3 horizontalStartPos = { GridStartPosition.x - halfCellSize,		GridStartPosition.y + ( CellSize * i ) - halfCellSize, 0.f };
				glm::vec3 horizontalEndPos = { GridEndPosition.x - halfCellSize,			GridStartPosition.y + ( CellSize * i ) - halfCellSize, 0.f };
				Graphics::GraphicsDraw::DrawWorldLine( horizontalStartPos, horizontalEndPos, Graphics::Colours::Blue );

				//Vertical Lines
				glm::vec3 verticalStartPos = { GridStartPosition.x + ( CellSize * i ) - halfCellSize,	GridStartPosition.y - halfCellSize,		0.f };
				glm::vec3 verticalEndPos = { GridStartPosition.x + ( CellSize * i ) - halfCellSize,	GridEndPosition.y - halfCellSize,			0.f };
				Graphics::GraphicsDraw::DrawWorldLine( verticalStartPos, verticalEndPos, Graphics::Colours::Red );
			}
		}

		//Get the mouse position
		glm::vec3 mousePos = m_inputSystem.GetMouseWorldPosition();

		if ( ( mousePos.x > 0 && mousePos.y > 0 ) && ( mousePos.x < StateComponent.GridSize && mousePos.y < StateComponent.GridSize ) )
		{
			//Floor it to within the grid tile positions
			//This is now a cell index use that to set the sand state on the array
			mousePos.x = glm::floor( mousePos.x / CellSize );
			mousePos.y = glm::floor( mousePos.y / CellSize );
			mousePos.z = 0.f;
			//Draw the mouse position as a cell, like a preview before we actually add a sand block.
			Graphics::GraphicsDraw::DrawWorldPoint( mousePos, 30.f, Graphics::Colours::Green );

			if ( m_inputSystem.IsPressesd( PlayerInputActions::AddSand ) && m_inputSystem.IsMouseWithinViewport() )
			{

					
				StateComponent.GridSandStateArray[static_cast< unsigned int >( mousePos.x )][static_cast< unsigned int >( mousePos.y )] = 1;
			}
		}

		//Place sand under a mouse press

		//Draw the sand
		for ( unsigned int i = 0; i < StateComponent.GridSize; ++i )
		{
			for ( unsigned int j = 0; j < StateComponent.GridSize; ++j )
			{
				unsigned int state = StateComponent.GridSandStateArray[i][j];
				if ( StateComponent.GridSandStateArray[i][j] > 0 )
				{
					glm::vec3 cellPosition = { GridStartPosition.x + ( CellSize * i ),	GridStartPosition.y + ( CellSize * j ),	0.f };
					glm::vec3 hsvColour( state, 255, 255 );
				
					glm::vec4 colour = { glm::rgbColor( hsvColour ), 1.f };
					Graphics::GraphicsDraw::DrawWorldPoint( cellPosition, 30.f, colour );
				}
			}
		}

	}

}
