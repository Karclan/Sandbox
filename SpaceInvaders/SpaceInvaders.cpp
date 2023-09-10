// SpaceInvaders.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GameImpl.h"

int main( [[maybe_unused]] int argc, [[maybe_unused]] char * argv[] )
{
	Game::SpaceInvadersGame Game = Game::SpaceInvadersGame();

	Game.EditorInit();
	Game.Init();



	while ( !Game.ShouldClose() )
	{
		Game.Run();
	}

	Game.DeInit();
	Game.DeEditorInit();
	return 0;
}