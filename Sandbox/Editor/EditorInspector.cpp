#include "EditorInspector.h"

#include "imgui.h"

#include "../Utils/UnusedMacro.h"
#include "../Utils/format.h"

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
			if ( entityListSize > 0 )
			{
				comboLabel = fmt::format( "{}", entityList[ entityListIndex ] );
			}
			if(ImGui::BeginCombo( "Entities" , comboLabel.c_str() ) )
			{
				for ( size_t i = 0; i < entityListSize; i++ )
				{
					bool const is_selected = ( entityListIndex == i );
					if ( ImGui::Selectable( fmt::format( "{}", entityList[ entityListIndex ] ).c_str(), is_selected ) )
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

			auto const storage = m_registry.GetStorage();
			for ( auto && curr : storage )
			{
				entt::id_type id = curr.first;

				if ( auto & storage = curr.second; storage.contains( m_InspectedEntity ) )
				{
					//std::cout << "Entity has component id = " << id << std::endl;
					// Or do whatever you will with the component ID
				}
			}

			ImGui::End();
		}
	}

	void EditorInspector::RegisterComponentDetailsInternal( entt::type_info componentTypeInfo, std::function<void()> && delegate )
	{
		m_DetailsViewDelegates.emplace( componentTypeInfo, delegate );
	}

}