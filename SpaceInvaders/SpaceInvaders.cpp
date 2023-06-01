// SpaceInvaders.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GameImpl.h"

int main( [[maybe_unused]] int argc, [[maybe_unused]] char * argv[] )
{
	Game::SpaceInvadersGame Game = Game::SpaceInvadersGame();

	Game.Init();
	Game.EditorInit();

	while ( !Game.ShouldClose() )
	{
		Game.Run();
	}

	Game.DeEditorInit();
	Game.DeInit();
	return 0;
}