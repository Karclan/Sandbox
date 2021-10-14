// SpaceInvaders.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core/Game.h"
#include "BasicMovementSubsystem.h"
#include "BasicSpriteTestSubsystem.h"
#include "ImguiWindow.h"
#include "VectorMathExampleSubsystem.h"

int main( [[maybe_unused]] int argc, [[maybe_unused]] char* argv [] )
{
	Core::Game Game;

	auto VecSub = std::make_unique<Game::VectorMathExampleSubsystem>( Game.GetRegistry() );
	VecSub->CreateComponents( *Game.GetWindow() );
	auto BasicSpriteSubsystem = std::make_unique<Game::BasicSpriteTestSubsystem>( Game.GetRegistry(), Game.GetWindow() );
	BasicSpriteSubsystem->CreateComponents( *Game.GetWindow() );

	//auto BasicMovementSubsystem = std::make_shared<Game::BasicMovementSubsystem>(Game.getRegistry());
	//BasicMovementSubsystem->createComponents(*Game.getWindow());

	Game.Init();
	Game.RegisterSubsystem( VecSub.get() );
	Game.RegisterImguiWindow( VecSub.get() );
	Game.RegisterSubsystem( BasicSpriteSubsystem.get() );
	Game.RegisterImguiWindow( BasicSpriteSubsystem.get() );

	//Game.registerSubsystem(BasicMovementSubsystem);
	//Game.registerImguiWindow(BasicMovementSubsystem);

	while ( !Game.ShouldClose() )
	{
		Game.Run();
	}

	Game.DeregisterImguiWindow( BasicSpriteSubsystem.get() );
	Game.DeregisterSubsystem( BasicSpriteSubsystem.get() );
	Game.DeregisterImguiWindow( VecSub.get() );
	Game.DeregisterSubsystem( VecSub.get() );

	return 0;
}