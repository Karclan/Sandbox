#include "GameImpl.h"

#include "ComponentDetails/GameComponentDetails.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/TransformComponent.h"

namespace Game
{

	SpaceInvadersGame::SpaceInvadersGame()
		: m_PlayerControllerSubsytem( GetRegistry(), GetWindow(), GetInputSystem() )
		, m_GridSubsystem( GetRegistry(), GetWindow() )
		, m_FallingSandSubsystem( GetRegistry(), GetInputSystem()  )
		, m_VectorMathExampleSubsystem( GetRegistry() )
	{

	}

	void SpaceInvadersGame::Init()
	{
		GameClass::Init();

		/*RegisterSubsystem( &m_PlayerControllerSubsytem );
		RegisterSubsystem( &m_GridSubsystem );
		RegisterSubsystem( &m_FallingSandSubsystem );*/

		RegisterSubsystem( &m_VectorMathExampleSubsystem );

		m_VectorMathExampleSubsystem.CreateComponents( GetWindow() );

		MouseBindings = std::make_shared<GameMouseBindings>();
		KeyboardBindings = std::make_shared<GameKeyboardBindings>();
		m_inputSystem.AddBinding( MouseBindings );
		m_inputSystem.AddBinding( KeyboardBindings );
	}

	void SpaceInvadersGame::EditorInit()
	{
		auto CameraEntity = m_cameraSystem.CreateOrthographicCamera( "EditorCamera" );
		m_cameraSystem.SetActiveCamera( "EditorCamera" );

		Graphics::CameraComponent & cameraComponent = m_mainRegistry.GetComponent<Graphics::CameraComponent>( CameraEntity );
		cameraComponent.m_farPlane = 100.f;
		cameraComponent.m_nearPlane = 0.1f;
		cameraComponent.m_orbit = false;
		cameraComponent.m_orthoAspectScale = 252.f;
		
		Graphics::TransformComponent & transformComponent = m_mainRegistry.GetComponent<Graphics::TransformComponent>( CameraEntity );
		transformComponent.Position.x = 250.f;
		transformComponent.Position.y = 250.f;
		transformComponent.Position.z = 10.f; //For the sake of switching between ortho and perspective

		glm::vec3 rotationEuler = glm::vec3( 180.f, 0.f, 0.f );
		transformComponent.RotationQuat = glm::radians( rotationEuler );
		GameClass::EditorInit();
	}

	void SpaceInvadersGame::DeEditorInit()
	{
		GameClass::DeEditorInit();
	}

	void SpaceInvadersGame::DeInit()
	{
		/*DeregisterSubsystem( &m_GridSubsystem );
		DeregisterSubsystem( &m_PlayerControllerSubsytem );
		DeregisterSubsystem( &m_FallingSandSubsystem );*/

		DeregisterSubsystem( &m_VectorMathExampleSubsystem );

		GameClass::DeInit();
	}

	void SpaceInvadersGame::RegisterComponentDetails()
	{
		GameClass::RegisterComponentDetails();
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::PlayerComponentDetails>() ) );
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::GameGridComponentDetails>() ) );

		m_editorInspector.RegisterTagDetailsView( std::move( std::make_unique<Editor::PlayerTagDetails>() ) );
	}
}
