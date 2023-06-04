#include "EditorInspector.h"

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
			std::string comboLabel ("no-entities");

			auto generateNameLabel = [this] ( Core::Entity entity, std::string & outLabel )
			{
				Generic::NameComponent const * NameComponent = m_registry.TryGetComponent<Generic::NameComponent const>( entity );
				if ( NameComponent )
				{
					outLabel = fmt::format( "Id: {} Name: {} ", entity, NameComponent->Name );
					if ( m_InspectedEntity== Core::NullEntity )
					{
						m_InspectedEntity = entity;
					}
				}
				else
				{
					outLabel = fmt::format( "Id: {} ", entity );
				}
			};

			if ( entityListSize > 0 )
			{
				Core::Entity entity = entityList[entityListIndex];
				generateNameLabel( entity, comboLabel );				
			}

			if(ImGui::BeginCombo( "Entities" , comboLabel.c_str() ) )
			{
				for ( size_t i = 0; i < entityListSize; i++ )
				{
					std::string label = "";
					Core::Entity entity = entityList[ i ];
					generateNameLabel( entity, label );

					bool const is_selected = ( entityListIndex == i );
					if ( ImGui::Selectable( label.c_str(), is_selected ) )
					{
						entityListIndex = i;
						m_InspectedEntity = entityList[ entityListIndex ];
					}

					if ( is_selected )  // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::BeginChild( "##ScrollingRegion", ImVec2( 0, 0 ), false, ImGuiWindowFlags_HorizontalScrollbar );
			//for all component storage containers
			auto const storage = m_registry.GetStorage();
			for ( auto && curr : storage )
			{
				//if this storage contains our selected entity
				if ( auto & storage = curr.second; storage.contains( m_InspectedEntity ) )
				{
					entt::id_type id = curr.first;
					//Generic::NameComponent const * NameComponent = m_registry.TryGetComponent<Generic::NameComponent const>( m_InspectedEntity );
					//Get the details view for the given component type
					DelegateMap::iterator detailDelegate = m_DetailsViewDelegates.find( id );
					if ( detailDelegate != m_DetailsViewDelegates.end() )
					{
						InfoDelegatePair const & Pair = detailDelegate->second;
						
						//Possibly very dangerous - storage.value(m_InspectedEntity) 
						Pair.DetailsClass->BuildDetailsWindow( storage.value(m_InspectedEntity) );
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