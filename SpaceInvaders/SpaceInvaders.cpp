// SpaceInvaders.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core/GameClass.h"
#include "BasicMovementSubsystem.h"
#include "BasicSpriteTestSubsystem.h"
#include "ImguiWindow.h"
#include "VectorMathExampleSubsystem.h"
#include "CollisionTestSubsystem.h"

#include "Basic2DGameTestSubsystem.h"


class SpaceInvadersGame : public Core::GameClass
{
public:
	SpaceInvadersGame()
		: m_BasicMovementSubsystem( GetRegistry() )
		, m_VectorMathExampleSubsystem( GetRegistry() )
	{

	}

	void Init() override
	{
		m_BasicMovementSubsystem.CreateComponents( GetWindow() );
		m_VectorMathExampleSubsystem.CreateComponents( GetWindow() );

		RegisterSubsystem( &m_BasicMovementSubsystem );
		RegisterSubsystem( &m_VectorMathExampleSubsystem );
		
		GameClass::Init();
	}
	void EditorInit() override
	{
		GameClass::EditorInit();
	}

	void DeEditorInit() override
	{
		GameClass::DeEditorInit();
	}

	void DeInit() override
	{
		DeregisterSubsystem( &m_BasicMovementSubsystem );
		DeregisterSubsystem( &m_VectorMathExampleSubsystem );
		DeregisterSubsystem( &m_VectorMathExampleSubsystem );
		GameClass::DeInit();
	}

protected:
	void RegisterComponentDetails() override
	{
		GameClass::RegisterComponentDetails();
	}

private:
	Game::BasicMovementSubsystem m_BasicMovementSubsystem;
	Game::VectorMathExampleSubsystem m_VectorMathExampleSubsystem;
};


int main( [[maybe_unused]] int argc, [[maybe_unused]] char * argv[] )
{
	SpaceInvadersGame Game = SpaceInvadersGame();

	//std::unique_ptr<Game::VectorMathExampleSubsystem> VecSub = std::move( std::make_unique<Game::VectorMathExampleSubsystem>( Game.GetRegistry() ) );
	//VecSub->CreateComponents( Game.GetWindow() );
	//std::unique_ptr<Game::BasicSpriteTestSubsystem> BasicSpriteSubsystem = std::move( std::make_unique<Game::BasicSpriteTestSubsystem>( Game.GetRegistry(), Game.GetWindow() ) );
	//BasicSpriteSubsystem->CreateComponents( Game.GetWindow() );
	//std::unique_ptr<Game::CollisionTestSubsystem> collisionTest = std::move( std::make_unique<Game::CollisionTestSubsystem>( Game.GetRegistry(), Game.GetInputSystem() ) );
	//std::unique_ptr <Game::BasicMovementSubsystem> BasicMovementSubsystem = std::make_unique<Game::BasicMovementSubsystem>( Game.GetRegistry() );
	//BasicMovementSubsystem->CreateComponents( Game.GetWindow() );
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
	//Game.RegisterSubsystem( BasicMovementSubsystem.get() );
	//Game.RegisterImguiWindow( BasicMovementSubsystem.get() );

	while ( !Game.ShouldClose() )
	{
		Game.Run();
	}

	Game.DeInit();

	//Game.DeregisterSubsystem( Basic2DSubSystem.get() );
	//Game.DeregisterImguiWindow( BasicSpriteSubsystem.get() );
	//Game.DeregisterSubsystem( BasicSpriteSubsystem.get() );
	//Game.DeregisterImguiWindow( VecSub.get() );
	//Game.DeregisterSubsystem( VecSub.get() );
	//Game.DeregisterSubsystem( BasicMovementSubsystem.get() );
	//Game.DeregisterSubsystem( collisionTest.get() );
	//Game.DeregisterImguiWindow( collisionTest.get() );

	return 0;
}