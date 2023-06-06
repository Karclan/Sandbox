#include "EditorInspector.h"
#include "EditorContextComponent.h"

#include "imgui.h"

#include "../Utils/UnusedMacro.h"
#include "../Utils/format.h"
#include "../Generic/NameComponent.h"

namespace Editor
{

	EditorInspector::EditorInspector( Core::Registry & registry )
		: Subsystem( registry )
	{

	}

	void EditorInspector::BuildWindow( double deltaTime )
	{
		UNUSED( deltaTime );

		//For a selected entity in the editor
		//Display all it's components
		
		if ( ImGui::Begin( "Inspector" ) )
		{
			ImGui::BeginChild( "##ScrollingRegion", ImVec2( 0, 0 ), false, ImGuiWindowFlags_HorizontalScrollbar );
			//for all component storage containers
			auto const & editorContextComponent = m_registry.ctx().get<EditorContextComponent const>();
			auto const storage = m_registry.GetStorage();
			for ( auto && curr : storage )
			{
				//if this storage contains our selected entity
				if ( auto & storage = curr.second; storage.contains( editorContextComponent.m_InspectedEntity ) )
				{
					entt::id_type id = curr.first;
					//Get the details view for the given component type
					DelegateMap::iterator detailDelegate = m_DetailsViewDelegates.find( id );
					if ( detailDelegate != m_DetailsViewDelegates.end() )
					{
						InfoDelegatePair const & Pair = detailDelegate->second;
						
						//Possibly very dangerous - "storage.value(m_InspectedEntity)" 
						Pair.DetailsClass->BuildDetailsWindow( storage.value( editorContextComponent.m_InspectedEntity ) );
					}
					else
					{
						if ( ImGui::CollapsingHeader( fmt::format( "Component Id Name: Unhandled Type Id: {}", id ).c_str() ) )
						{
							ImGui::Text("Add a component details for this" );
						}
					}
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}

	void EditorInspector::RegisterComponentDetailsInternal( entt::type_info componentTypeInfo, std::unique_ptr<ComponentDetails> && detailsClass )
	{
		m_DetailsViewDelegates.emplace( componentTypeInfo.hash(), InfoDelegatePair { componentTypeInfo , std::move(detailsClass) } );
	}

}