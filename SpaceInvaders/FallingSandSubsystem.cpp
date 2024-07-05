#include "FallingSandSubsystem.h"

#include "Utils/UnusedMacro.h"
#include "FallingSandSimulationComponent.h"
#include "Graphics/GraphicsDraw.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/common.hpp"
#include "GameInputActions.h"

#include "Utils/Logger.h"

namespace Game
{
	//Points are screen sized drawn.. Not world sized, we could do some fancy conversion to go from the grid Z position, the size of the cells and then figure it out for each sand pixel.. ORRRR we could just draw squares at a Z value and it'll work better, though we need to make a new "draw square" function, which also positions the square center of the coord we provide
	//https://stackoverflow.com/questions/25780145/gl-pointsize-corresponding-to-world-space-size - This is the fix for drawing points scaled WITHIN the vertex shader based on the unit size of the cell AND distance from them.
	//Also copied into Obsidian vault for reference

	FallingSandSubsystem::FallingSandSubsystem( Core::Registry & registry, Input::InputSystem & inputSystem )
		: Core::Subsystem( registry )
		, m_inputSystem( inputSystem )
	{
		InitComponents();
	}

	void FallingSandSubsystem::InitComponents()
	{
		std::srand( static_cast< unsigned int >( std::time( nullptr ) ) );

		StateEntity = m_registry.CreateEntity();
		Game::FallingSandStateComponent & StateComponent = m_registry.AddComponent<Game::FallingSandStateComponent>( StateEntity );

		for ( unsigned int i = 0; i < StateComponent.GridSize; ++i )
		{
			for ( unsigned int j = 0; j < StateComponent.GridSize; ++j )
			{
				StateComponent.GridSandStateArray[i][j] = 0.f;
			}
		}
		//Test sand!
		StateComponent.Gravity = glm::vec2( 0.f, 1.f );
	}

	void FallingSandSubsystem::FixedUpdate( double deltaTime )
	{
		UNUSED( deltaTime );

		auto & StateComponent = m_registry.GetComponent<Game::FallingSandStateComponent>( StateEntity );

		//Create a grid array to use
		//Zero it out
		float nextGrid[StateComponent.GridSize][StateComponent.GridSize];
		for ( unsigned int i = 0; i < StateComponent.RowsLength; ++i )
		{
			for ( unsigned int j = 0; j < StateComponent.ColumnsLength; ++j )
			{
				nextGrid[i][j] = 0.f;
			}
		}

		//Go through the grid
		for ( unsigned int i = 0; i < StateComponent.RowsLength; ++i )
		{
			for ( unsigned int j = 0; j < StateComponent.ColumnsLength; ++j )
			{
				//get the state of the sand
				float currentSand = StateComponent.GridSandStateArray[i][j];
				//If the state of the sand is above 0 (i.e set)
				if ( currentSand > 0.f )
				{
					//Make sure we don't index outside of the array
					if ( j + 1 < StateComponent.ColumnsLength )
					{
						bool resolved = false;

						float belowSand = StateComponent.GridSandStateArray[i][j + 1];//Get the state of the sand below

						if ( belowSand == 0 )//if the sand below is empty
						{
							nextGrid[i][j + 1] = currentSand; //This moves it down
							resolved = true;
						}

						//Need to add randomness to this
						int dir = 1;
						int randNumber = 1 + std::rand() / ( ( RAND_MAX + 1u ) / 6 );
						if ( randNumber > 3 )
						{
							dir *= -1;
						}

						if ( !resolved && i + dir < StateComponent.RowsLength )
						{
							float aSand = StateComponent.GridSandStateArray[i + dir][j + 1];
							if ( aSand == 0 )
							{
								nextGrid[i + dir][j + 1] = currentSand;
								resolved = true;
							}

						}

						if ( !resolved && i > 0 )
						{
							float bSand = StateComponent.GridSandStateArray[i - dir][j + 1];
							if ( bSand == 0 )
							{
								nextGrid[i - dir][j + 1] = currentSand;
								resolved = true;
							}

						}

						if ( !resolved )
							//else sand below isn't empty
						{
							nextGrid[i][j] = currentSand; //This makes it stack
							resolved = true;
						}
					}
					else //We're at the bottom of the array so just copy the state
					{
						nextGrid[i][j] = currentSand; //This stops it at the bottom
					}
				}
			}
		}

		//Copy next grid into the grid
		for ( unsigned int i = 0; i < StateComponent.RowsLength; ++i )
		{
			for ( unsigned int j = 0; j < StateComponent.ColumnsLength; ++j )
			{
				StateComponent.GridSandStateArray[i][j] = nextGrid[i][j];
			}
		}
	}

	void FallingSandSubsystem::Update( double deltaTime )
	{
		UNUSED( deltaTime );

		auto & StateComponent = m_registry.GetComponent<Game::FallingSandStateComponent>( StateEntity );

		unsigned int const & CellSize = StateComponent.CellSize;
		float const halfCellSize = static_cast< float >( CellSize ) / 2.f;
		glm::ivec2 & GridStartPosition = StateComponent.GridStartPosition;
		glm::ivec2 const GridEndPosition = { GridStartPosition.x + ( CellSize * StateComponent.GridSize ), GridStartPosition.y + ( CellSize * StateComponent.GridSize ) };

		////Draw the grid
		//{
		//	for ( int i = 0; i <= StateComponent.GridSize; ++i )
		//	{
		//		//Horizontal Lines
		//		glm::vec3 horizontalStartPos = { GridStartPosition.x - halfCellSize,		GridStartPosition.y + ( CellSize * i ) - halfCellSize, 0.f };
		//		glm::vec3 horizontalEndPos = { GridEndPosition.x - halfCellSize,			GridStartPosition.y + ( CellSize * i ) - halfCellSize, 0.f };
		//		Graphics::GraphicsDraw::DrawWorldLine( horizontalStartPos, horizontalEndPos, Graphics::Colours::Blue );

		//		//Vertical Lines
		//		glm::vec3 verticalStartPos = { GridStartPosition.x + ( CellSize * i ) - halfCellSize,	GridStartPosition.y - halfCellSize,		0.f };
		//		glm::vec3 verticalEndPos = { GridStartPosition.x + ( CellSize * i ) - halfCellSize,	GridEndPosition.y - halfCellSize,			0.f };
		//		Graphics::GraphicsDraw::DrawWorldLine( verticalStartPos, verticalEndPos, Graphics::Colours::Red );
		//	}
		//}

		//Get the mouse position
		glm::vec3 mousePos = m_inputSystem.GetMouseWorldPosition();

		static float vibrance = 0.f;
		if ( ( mousePos.x > 0 && mousePos.y > 0 ) && ( mousePos.x < StateComponent.GridSize && mousePos.y < StateComponent.GridSize ) )
		{
			//Floor it to within the grid tile positions
			//This is now a cell index use that to set the sand state on the array
			mousePos.x = glm::floor( mousePos.x / CellSize );
			mousePos.y = glm::floor( mousePos.y / CellSize );
			mousePos.z = 0.f;
			//Draw the mouse position as a cell, like a preview before we actually add a sand block.
			Graphics::GraphicsDraw::DrawWorldPoint( mousePos, 1.f, Graphics::Colours::Green );

			if ( m_inputSystem.IsHeld( PlayerInputActions::AddSand ) && m_inputSystem.IsMouseWithinViewport() )
			{
				const int mouseXIndex = static_cast< unsigned int >( mousePos.x );
				const int mouseYIndex = static_cast< unsigned int >( mousePos.y );
				//Place sand under a mouse press
				StateComponent.GridSandStateArray[mouseXIndex][mouseYIndex] = vibrance;

				//left and right of cursor
				if ( mouseXIndex > 0 )
				{
					StateComponent.GridSandStateArray[mouseXIndex - 1][mouseYIndex] = vibrance;
				}

				if ( mouseXIndex + 1 > StateComponent.GridSize )
				{
					StateComponent.GridSandStateArray[mouseXIndex + 1][mouseYIndex] = vibrance;
				}

				//above and below of cursor
				if ( mouseYIndex + 1 > StateComponent.GridSize )
				{
					StateComponent.GridSandStateArray[mouseXIndex][mouseYIndex + 1] = vibrance;
				}
				if ( mouseYIndex > 0 )
				{
					StateComponent.GridSandStateArray[mouseXIndex][mouseYIndex - 1] = vibrance;
				}

				vibrance += 0.1f;
				if ( vibrance > 360.f )
				{
					vibrance = 0.f;
				}
			}
		}

		//Draw the sand
		for ( unsigned int i = 0; i < StateComponent.GridSize; ++i )
		{
			for ( unsigned int j = 0; j < StateComponent.GridSize; ++j )
			{
				float state = StateComponent.GridSandStateArray[i][j];
				if ( StateComponent.GridSandStateArray[i][j] > 0 )
				{
					glm::vec3 cellPosition = { GridStartPosition.x + ( CellSize * i ),	GridStartPosition.y + ( CellSize * j ),	0.f };
					glm::vec3 colour( state, 1.f, 1.f );
					glm::vec4 converted( glm::rgbColor( colour ), 1.f );

					Graphics::GraphicsDraw::DrawWorldPoint( cellPosition, 1.f, converted );
				}
			}
		}


	}
}
