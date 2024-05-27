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
		for ( int * sandState : StateComponent.GridSandStateArray )
		{
			*sandState = 0;
		}
		StateComponent.Gravity = glm::vec2( 0.f, 1.f );
	}

	void FallingSandSubsystem::FixedUpdate( double deltaTime )
	{
		UNUSED( deltaTime );
	}

	void FallingSandSubsystem::Update( double deltaTime )
	{
		UNUSED( deltaTime );

		//Draw the grid
		auto & StateComponent = m_registry.GetComponent<Game::FallingSandStateComponent>( StateEntity );

		unsigned int const & CellSize = StateComponent.CellSize;
		glm::ivec2 & GridStartPosition = StateComponent.GridStartPosition;

		glm::ivec2 const GridEndPosition = { GridStartPosition.x + ( CellSize * StateComponent.GridSize ), GridStartPosition.y + ( CellSize * StateComponent.GridSize ) };
		for ( int i = 0; i <= StateComponent.GridSize; ++i )
		{
			//Horizontal Lines
			glm::vec3 horizontalStartPos = { GridStartPosition.x, GridStartPosition.y + ( CellSize * i ), 0.f };
			glm::vec3 horizontalEndPos =   { GridEndPosition.x, GridStartPosition.y + ( CellSize * i ), 0.f };
			Graphics::GraphicsDraw::DrawWorldLine( horizontalStartPos, horizontalEndPos, Graphics::Colours::Blue );
			//Vertical Lines
			glm::vec3 verticalStartPos = { GridStartPosition.x + ( CellSize * i ), GridStartPosition.y, 0.f };
			glm::vec3 verticalEndPos = { GridStartPosition.x + ( CellSize * i ), GridEndPosition.y, 0.f };
			Graphics::GraphicsDraw::DrawWorldLine( verticalStartPos, verticalEndPos, Graphics::Colours::Red );
		}


		//Get the mouse position
		glm::vec3 mousePos = m_inputSystem.GetMouseWorldPosition();
		//Floor it to within the grid tile positions
		mousePos.x = glm::floor( mousePos.x / CellSize );
		mousePos.y = glm::floor( mousePos.y / CellSize );
		mousePos.z = -100.f;
		Graphics::GraphicsDraw::DrawWorldPoint( mousePos, 30.f, Graphics::Colours::Green );


		if ( m_inputSystem.IsPressesd( PlayerInputActions::AddSand ) && m_inputSystem.IsMouseWithinViewport() )
		{

			//mousePos
			//This is now a cell index use that to set the sand state on the array
		
		}
		
		//Draw the sand
	}

}
