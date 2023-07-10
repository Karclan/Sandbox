#include "GameClass.h"

#include "Subsystem.h"

#include "../Graphics/Window.h"

#include "../Graphics/ComponentDetails/GraphicsComponentsDetails.h"
#include "../Physics/ComponentDetails/PhysicsComponentsDetails.h"
#include "../Generic/ComponentDetails/GenericComponentsDetails.h"
#include "../Graphics/ComponentDetails/GraphicsTagsDetails.h"

#include "../Editor/EditorContextComponent.h"

#include "imgui.h"

namespace Core
{
	static constexpr double fixedUpdateRate = 1.f / 60.f;

	GameClass::GameClass()
		: m_editorInspector( m_mainRegistry )
		, m_editorHierarchyWindow( m_mainRegistry )
		, m_editorStatsViewer( m_mainRegistry )
		, m_inputSystem( m_mainRegistry )
		, m_logWindow( m_mainRegistry )
		, m_cameraSystem( m_mainRegistry )
		, m_window( m_mainRegistry, m_inputSystem )
		, m_collisionDetectionSystem( m_mainRegistry, m_window )
	{
		m_baseImguiWindowBuilder = std::make_unique<Core::BaseImGuiWindowBuilder>( m_mainRegistry, m_window );
	}

	GameClass::~GameClass() = default;

	void GameClass::Init()
	{
		RegisterSubsystem( &m_logWindow );
		RegisterSubsystem( &m_inputSystem );
		RegisterSubsystem( &m_cameraSystem );
		RegisterSubsystem( &m_collisionDetectionSystem );

		RegisterSubsystem( &m_window.GetRenderer() );

		m_baseImguiWindowBuilder->GetInfo();
	}

	void GameClass::EditorInit()
	{
		m_mainRegistry.ctx().emplace<Editor::EditorContextComponent>();

		/*Core::Entity CameraEntity = EditorUtilities.CreatePerspectiveCamera( "MainCamera" );
		m_cameraSystemRef.SetActiveCamera( "MainCamera" );*/
		//m_editorUtilities.CreateEditorCamera( CameraEntity, m_mainRegistry );
		/*auto CameraEntity = m_cameraSystem.CreatePerspectiveCamera( "EditorCamera" );
		m_cameraSystem.SetActiveCamera( "EditorCamera" );*/
		KeyboardBindings = std::make_shared<Editor::EditorKeyboardBindings>();
		MouseBindings = std::make_shared<Editor::EditorMouseBindings>();
		m_inputSystem.AddBinding( MouseBindings );
		m_inputSystem.AddBinding( KeyboardBindings );

		RegisterSubsystem( &m_editorInspector );
		RegisterSubsystem( &m_editorHierarchyWindow );
		RegisterSubsystem( &m_editorStatsViewer );

		RegisterComponentDetails();
	}

	void GameClass::DeInit()
	{
		DeregisterSubsystem( &m_inputSystem );
		DeregisterSubsystem( &m_cameraSystem );
		DeregisterSubsystem( &m_collisionDetectionSystem );

		DeregisterSubsystem( &m_inputSystem );
		DeregisterSubsystem( &m_cameraSystem );

		DeregisterSubsystem( &m_logWindow );
		DeregisterSubsystem( &m_window.GetRenderer() );
	}

	void GameClass::DeEditorInit()
	{
		DeregisterSubsystem( &m_editorInspector );
		DeregisterSubsystem( &m_editorHierarchyWindow );
		DeregisterSubsystem( &m_editorStatsViewer );
	}

	void GameClass::RegisterSubsystemInternal( Subsystem * subsystem )
	{
		//#TODO: Sort based on update order
		m_subsystems.push_back( subsystem );
	}

	void GameClass::RegisterImguiWindowInternal( ImGuiWindowBuilder * window )
	{
		m_imguiWindows.push_back( window );
	}

	void GameClass::DeregisterSubsystemInternal( Subsystem * subsystem ) noexcept
	{
		for ( auto it = m_subsystems.begin(); it != m_subsystems.end(); ++it )
		{
			if ( *it == subsystem )
			{
				m_subsystems.erase( it );
				break;
			}
		}
	}

	void GameClass::DeregisterImguiWindowInternal( ImGuiWindowBuilder * subsystem ) noexcept
	{
		for ( auto it = m_imguiWindows.begin(); it != m_imguiWindows.end(); ++it )
		{
			if ( *it == subsystem )
			{
				m_imguiWindows.erase( it );
				break;
			}
		}
	}

	bool GameClass::ShouldClose() const noexcept
	{
		return m_window.ShouldClose();
	}

	void GameClass::Run()
	{
		//update input
		m_window.PollInput();

		m_window.PreRender();

		double const frameTime = m_window.GetDeltaTime();

		m_baseImguiWindowBuilder->BuildWindow( frameTime );
		for ( auto & imguiWindow : m_imguiWindows )
		{
			imguiWindow->BuildWindow( frameTime );
		}

		m_accumulator += frameTime;

		int const framesToCatchUpOn = static_cast< int >( m_accumulator / fixedUpdateRate );

		m_accumulator = m_accumulator;
		m_framesToCatchUpOn = framesToCatchUpOn;

		while ( m_accumulator >= fixedUpdateRate )
		{
			for ( auto & subSystem : m_subsystems )
			{
				subSystem->FixedUpdate( fixedUpdateRate );
			}
			m_accumulator -= fixedUpdateRate;
		}

		for ( auto & subSystem : m_subsystems )
		{
			subSystem->Update( frameTime );
		}

		m_window.Render();
	}

	void GameClass::RegisterComponentDetails()
	{
		m_editorInspector.RegisterNameComponentDetailsView( std::move( std::make_unique<Editor::NameComponentDetails>() ) );

		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::TransformComponentDetails>() ) );
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::CameraComponentDetails>() ) );

		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::CollisionComponentDetails>() ) );
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::CircleExtentsDetails>() ) );
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::SquareExtentsDetails>() ) );
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::CircleExtentsDebugRenderComponentDetails>() ) );
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::SquareExtentsDebugRenderComponentDetails>() ) );

		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::MeshComponentDetails>() ) );
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::ShaderComponentDetails>() ) );
		m_editorInspector.RegisterComponentDetailsView( std::move( std::make_unique<Editor::TextureComponentDetails>() ) );

		m_editorInspector.RegisterTagDetailsView( std::move( std::make_unique<Editor::ActiveCameraTagDetails>() ) );
	}

	void GameClass::SortSubsytems() noexcept
	{

	}
}