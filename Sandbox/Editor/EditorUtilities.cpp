#include "EditorUtilities.h"

#include "../Core/CoreRegistry.h"

#include "../Graphics/TransformComponent.h"
#include "../Graphics/CameraComponent.h"
#include "../Graphics/ShaderComponent.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/GraphicsTags.h"

namespace Editor
{

	void Utilities::CreateEditorCamera( Core::Entity entity, Core::Registry& Registry)
	{
		if ( Registry.IsValidEntity( m_editorCameraEntity ) )
		{
			Registry.DestroyEntity( m_editorCameraEntity );
		}

		m_editorCameraEntity = entity;
		
		Graphics::TransformComponent & transformComponent = Registry.GetComponent<Graphics::TransformComponent>( m_editorCameraEntity );
		transformComponent.Position.z = -10.f;

		auto ActiveCameraTagView = Registry.View<Graphics::ActiveCameraTag>();
		if ( ActiveCameraTagView.size() == 0 )
		{
			Registry.AddComponent<Graphics::ActiveCameraTag>( m_editorCameraEntity );
		}
		
		//Registry.AddComponent<ArcballComponent>( m_editorCameraEntity );
		//Create components for editor camera
		//Transform
		//Camera component
		//Custom arcball camera behavior component to be picked up by the system
	}

	void Utilities::CreateGridFloor( Core::Registry& Registry, Graphics::Renderer & Renderer )
	{
		if ( Registry.IsValidEntity( m_gridFloorEntity ) )
		{
			Registry.DestroyEntity( m_gridFloorEntity );
		}

		Renderer.GetOrCreateShader( "EditorGridFloor", { Graphics::Shader::Type::Vertex, Graphics::Shader::Type::Fragment } );

		m_gridFloorEntity = Registry.CreateEntity();
		
		Registry.AddComponent<Graphics::ShaderComponent>( m_gridFloorEntity );
		//Registry.AddComponent<Editor::GridFloorComponent>( m_gridFloorEntity );
		//Create components for grid floor
		//Shader
		//Custom component needed for any parameters to pass in
	}

}