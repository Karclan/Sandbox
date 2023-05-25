// SpaceInvaders.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core/Game.h"
#include "BasicMovementSubsystem.h"
#include "BasicSpriteTestSubsystem.h"
#include "ImguiWindow.h"
#include "VectorMathExampleSubsystem.h"
#include "CollisionTestSubsystem.h"

#include "Basic2DGameTestSubsystem.h"

int main( [[maybe_unused]] int argc, [[maybe_unused]] char * argv[] )
{
	Core::Game Game = Core::Game();

	//std::unique_ptr<Game::VectorMathExampleSubsystem> VecSub = std::move( std::make_unique<Game::VectorMathExampleSubsystem>( Game.GetRegistry() ) );
	//VecSub->CreateComponents( Game.GetWindow() );
	//std::unique_ptr<Game::BasicSpriteTestSubsystem> BasicSpriteSubsystem = std::move( std::make_unique<Game::BasicSpriteTestSubsystem>( Game.GetRegistry(), Game.GetWindow() ) );
	//BasicSpriteSubsystem->CreateComponents( Game.GetWindow() );

	//std::unique_ptr<Game::CollisionTestSubsystem> collisionTest = std::move( std::make_unique<Game::CollisionTestSubsystem>( Game.GetRegistry(), Game.GetInputSystem() ) );
	std::unique_ptr <Game::BasicMovementSubsystem> BasicMovementSubsystem = std::make_unique<Game::BasicMovementSubsystem>( Game.GetRegistry() );
	BasicMovementSubsystem->CreateComponents( Game.GetWindow() );

	//std::unique_ptr<Game::Basic2DGameTestSubsystem> Basic2DSubSystem = std::move( std::make_unique< Game::Basic2DGameTestSubsystem>( Game.GetRegistry(), Game.GetWindow() , Game.GetCameraSystem()) );
	//Basic2DSubSystem->Init();
	Game.Init();
	Game.EditorInit();
	//Game.RegisterSubsystem( Basic2DSubSystem.get() );
	//Game.RegisterSubsystem( VecSub.get() );
	//Game.RegisterImguiWindow( VecSub.get() );
	//Game.RegisterSubsystem( BasicSpriteSubsystem.get() );
	//Game.RegisterImguiWindow( BasicSpriteSubsystem.get() );
	//Game.RegisterSubsystem( collisionTest.get() );
	//Game.RegisterImguiWindow( collisionTest.get() );

	Game.RegisterSubsystem( BasicMovementSubsystem.get() );
	//Game.RegisterImguiWindow( BasicMovementSubsystem.get() );

	while ( !Game.ShouldClose() )
	{
		Game.Run();
	}

	//Game.DeregisterSubsystem( Basic2DSubSystem.get() );

	//Game.DeregisterImguiWindow( BasicSpriteSubsystem.get() );
	//Game.DeregisterSubsystem( BasicSpriteSubsystem.get() );
	//Game.DeregisterImguiWindow( VecSub.get() );
	//Game.DeregisterSubsystem( VecSub.get() );

	Game.DeregisterSubsystem( BasicMovementSubsystem.get() );
	//Game.DeregisterSubsystem( collisionTest.get() );
	//Game.DeregisterImguiWindow( collisionTest.get() );

	return 0;
}