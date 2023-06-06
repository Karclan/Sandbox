#include "EditorHierarchyWindow.h"

#include "EditorContextComponent.h"

#include "../Utils/UnusedMacro.h"
#include "../Utils/format.h"

#include "imgui.h"
#include "ImGui/Widgets.h"

namespace Editor
{
	EditorHierarchyWindow::EditorHierarchyWindow( Core::Registry & registry )
		: Core::Subsystem( registry )
	{
	}

	void EditorHierarchyWindow::BuildWindow( double deltaTime )
	{
		UNUSED( deltaTime );
		
		if ( ImGui::Begin( "Hierarchy" ) )
		{
			//Can change this later to instead have an inspector view that lists entities
			//we will need to do some filtering on this as well so we only have entities which have some sort of "inspectable" tag
			//as we'll have the inspector view go nuts if we have lots of entities creating/destroying
			size_t const entityCount = m_registry.GetStorage<Core::Entity>().in_use();
			std::vector<Core::Entity> entityList;
			entityList.reserve( entityCount );
			m_registry.Each( [&entityList] ( auto entity )
				{
					entityList.emplace_back( entity );
				} );
			entityList.shrink_to_fit();

			static size_t entityListIndex = 0;
			const size_t entityListSize = entityList.size();
			std::string comboLabel( "no-entities" );

			auto generateNameLabel = [this] ( Core::Entity entity )
			{
				Generic::NameComponent const * NameComponent = m_registry.TryGetComponent<Generic::NameComponent const>( entity );
				if ( NameComponent )
				{
					return fmt::format( "Id: {} Name: {} ", entity, NameComponent->Name );
				}
				else
				{
					return fmt::format( "Id: {} ", entity );
				}
			};

			auto setContextEntity = [this] ()
			{
				auto & editorContextComponent = m_registry.ctx().get<EditorContextComponent>();
				editorContextComponent.m_InspectedEntity = m_selectedEntity;
			};

			if ( entityListSize > 0 )
			{
				Core::Entity entity = entityList[ entityListIndex ];
				comboLabel = generateNameLabel( entity );

				if ( m_selectedEntity == Core::NullEntity )
				{
					m_selectedEntity = entity;
					auto editorContextComponent = m_registry.ctx().get<EditorContextComponent>();
					//#TODO: Maybe change this to be an event?
					editorContextComponent.m_InspectedEntity = m_selectedEntity;
				}

				setContextEntity();
			}

			for ( size_t i = 0; i < entityListSize; i++ )
			{
				Core::Entity entity = entityList[ i ];
				std::string const label = generateNameLabel( entity );
				bool const hasChildren = false; //#TODO_Entity_Parenting: Find if this entity has children
				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
				const bool is_selected = m_selectedEntity == entityList[ i ];
				if ( is_selected )
				{
					node_flags |= ImGuiTreeNodeFlags_Selected;
				}

				//Parenting notes: Unity will set the transform to be effectively the same position it is without parenting
				if ( !hasChildren ) //if no child entities //#TODO_Entity_Parenting
				{
					node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				}

				//#TODO_EditorIcons: Hierarchy entity icons - maybe buttons too
				/*ImGui::Image( "[]" );*/
				/*ImGui::SameLine();*/
				bool node_open = ImGui::TreeNodeEx( ( void * ) ( intptr_t ) i, node_flags, label.c_str(), i );
				
				if ( ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() )
				{
					m_selectedEntity = entity;
				}
				if ( ImGui::BeginDragDropSource() )
				{
					ImGui::SetDragDropPayload( "ENTITYNODE", NULL, 0 );
					ImGui::Text( label.c_str() );
					ImGui::EndDragDropSource();
				}
				if ( node_open && hasChildren )
				{
					ImGui::BulletText( "Some possible child entities" );
					ImGui::TreePop();
				}

			}

		}
		ImGui::End();
	}
}
