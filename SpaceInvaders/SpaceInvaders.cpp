// SpaceInvaders.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core/Game.h"
#include "BasicMovementSubsystem.h"
#include "BasicSpriteTestSubsystem.h"
#include "ImguiWindow.h"
#include "VectorMathExampleSubsystem.h"

int main( int argc, char * argv [] )
{
	Core::Game Game;

	//std::shared_ptr<Game::VectorMathExampleSubsystem> VecSub = std::make_shared<Game::VectorMathExampleSubsystem>(Game.getRegistry());
	//VecSub->CreateComponents(*Game.getWindow());
	auto BasicSpriteSubsystem = std::make_shared<Game::BasicSpriteTestSubsystem>(Game.GetRegistry(), Game.GetWindow());
	BasicSpriteSubsystem->CreateComponents(*Game.GetWindow());

	//auto BasicMovementSubsystem = std::make_shared<Game::BasicMovementSubsystem>(Game.getRegistry());
	//BasicMovementSubsystem->createComponents(*Game.getWindow());

	Game.Init();
	//Game.registerSubsystem(VecSub);
	//Game.registerImguiWindow(VecSub);
	Game.RegisterSubsystem(BasicSpriteSubsystem);
	Game.RegisterImguiWindow(BasicSpriteSubsystem);

	//Game.registerSubsystem(BasicMovementSubsystem);
	//Game.registerImguiWindow(BasicMovementSubsystem);

	while (!Game.ShouldClose())
	{
		Game.Run();
	}

	return 0;
}