#include "Utilities.h"

#include "../Core/CoreRegistry.h"

#include "../Graphics/TransformComponent.h"
#include "../Graphics/CameraComponent.h"
#include "../Graphics/ShaderComponent.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Renderer.h"

namespace Editor
{

	void Utilities::CreateEditorCamera( Core::Registry& Registry )
	{
		if ( Registry.IsValidEntity( m_editorCameraEntity ) )
		{
			Registry.DestroyEntity( m_editorCameraEntity );
		}

		m_editorCameraEntity = Registry.CreateEntity();
		Graphics::TransformComponent transformComponent;
		transformComponent.Position.z = -10.f;
		Registry.AddComponent<Graphics::TransformComponent>( m_editorCameraEntity, transformComponent );
		Graphics::CameraComponent cameraComponent;
		cameraComponent.m_name = "Default";
		Registry.AddComponent<Graphics::CameraComponent>( m_editorCameraEntity, cameraComponent );
		Registry.AddComponent<Graphics::ActiveCameraTag>( m_editorCameraEntity );
		//Registry.AddComponent<ArcballComponent>( m_editorCameraEntity );
		//Create components for editor camera
		//Transform
		//Camera component
		//Custom arcball camera behaviour component to be picked up by the system
	}

	void Utilities::CreateGridFloor( Core::Registry& Registry, Graphics::Renderer & Renderer )
	{
		if ( Registry.IsValidEntity( m_gridFloorEntity ) )
		{
			Registry.DestroyEntity( m_gridFloorEntity );
		}

		Renderer.CreateShader( "EditorGridFloor", { Graphics::Shader::Type::Vertex, Graphics::Shader::Type::Fragment } );

		m_gridFloorEntity = Registry.CreateEntity();
		
		Registry.AddComponent<Graphics::ShaderComponent>( m_gridFloorEntity );
		//Registry.AddComponent<Editor::GridFloorComponent>( m_gridFloorEntity );
		//Create compoents for grid floor
		//Shader
		//Custom component needed for any parameters to pass in
	}

}