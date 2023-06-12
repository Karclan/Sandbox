#include "EditorInspector.h"
#include "EditorContextComponent.h"

#include "imgui.h"
#include "ImGui/Widgets.h"

#include "../Utils/UnusedMacro.h"
#include "../Utils/format.h"
#include "../Generic/NameComponent.h"

#include <vector>

namespace Editor
{

	EditorInspector::EditorInspector( Core::Registry & registry )
		: Subsystem( registry )
		, m_NameComponentDetails()
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

			struct NameComponentHolder
			{
				void * nameComponent = nullptr;
			};
			struct ComponentPtrDetailsPair
			{
				size_t Order = 0;
				void * Component = nullptr;
				ComponentDetails * Details = nullptr;
			};
			struct FoundTagDetails
			{
				TagDetails * Details = nullptr;
			};
			struct UnmatchedDetails
			{
				entt::id_type Id = {};
			};

			NameComponentHolder nameComponentHolder;
			std::vector<ComponentPtrDetailsPair> foundComponentDetails;
			std::vector<FoundTagDetails> foundTagDetails;
			std::vector<UnmatchedDetails> unmatchedDetails;

			auto const & editorContextComponent = m_registry.ctx().get<EditorContextComponent const>();
			auto const storage = m_registry.GetStorage();
			for ( auto && curr : storage )//Go over all the component storage
			{
				//if this storage contains our selected entity
				if ( auto & storage = curr.second; storage.contains( editorContextComponent.m_InspectedEntity ) )
				{
					bool detailMatch = false;
					entt::id_type const id = curr.first;

					//Special case for the name component
					if ( id == m_NameComponentDetails.Info.hash() )
					{
						nameComponentHolder.nameComponent = storage.value( editorContextComponent.m_InspectedEntity );
						detailMatch = true;
						continue; //Skip rest of checks
					}

					//Component Details Search
					ComponentDetailsDelegateMap::iterator detailDelegate = m_ComponentDetailsViewDelegates.find( id );
					if ( detailDelegate != m_ComponentDetailsViewDelegates.end() )
					{
						ComponentDetailsDelegatePair const & Pair = detailDelegate->second;

						foundComponentDetails.push_back( { Pair.Order, storage.value( editorContextComponent.m_InspectedEntity ), Pair.DetailsClass.get(), } );

						detailMatch = true;
						continue; //Skip rest of checks
					}
					
					//Tag Search
					TagDetailsDelegateMap::const_iterator tagDelegate = m_TagDetailsViewDelegates.find( id );
					if ( tagDelegate != m_TagDetailsViewDelegates.end() )
					{
						TagDelegatePair const & Pair = tagDelegate->second;
						foundTagDetails.push_back( { tagDelegate->second.DetailsClass.get() } );
						detailMatch = true;
						continue; //Skip rest of checks
					}
					
					//Unmatched Search!
					if ( !detailMatch )
					{
						unmatchedDetails.push_back( { id } );
					}
				}
			}

			//Sort the component details pairs based on the order we have initially added these detail window delegates to the inspector 
			//Ideally this could be better, some sort of container that auto sorts on insertion rather than searching at the end. The order will not change after launch (unless we do dynamic modules or something)
			//So having a fixed insertion would be better
			std::sort( foundComponentDetails.begin(), foundComponentDetails.end(), [] ( ComponentPtrDetailsPair const & Left, ComponentPtrDetailsPair const & Right) -> bool
				{
					return Left.Order < Right.Order;
				}
			);
	
			if ( nameComponentHolder.nameComponent )
			{
				m_NameComponentDetails.DetailsClass->BuildDetailsWindow( nameComponentHolder.nameComponent );
			}
			else //Maybe have the ability to dynamically name something?
			{

			}

			//For all the tag details we have
			//Get the matching tags
			for ( FoundTagDetails const & tag : foundTagDetails )
			{
				//Combo selection of all the tags we could have?
			}

			std::string tagComboLabel = "";
			static int tagSelectionIndex = -1;
			size_t tagCount = foundTagDetails.size();
			if ( ImGui::BeginCombo( "Tags", tagComboLabel.c_str() ) )
			{
				for ( size_t i = 0; i < tagCount; i++ )
				{
					FoundTagDetails const & details = foundTagDetails[ i ];
					std::string const label = fmt::format( "{}", details.Details->GetTagName() );
					bool const is_selected = ( tagSelectionIndex == i );
					if ( ImGui::Selectable( label.c_str(), is_selected ) )
					{
						tagSelectionIndex = static_cast< int >( i );
					}
					if ( is_selected )  // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			//For all details
			for ( ComponentPtrDetailsPair const & componentDetails : foundComponentDetails )
			{
				//Possibly very dangerous - "componentDetails.Component" 
				componentDetails.Details->BuildDetailsWindow( componentDetails.Component );
			}
			//For all unmatched details
			for ( UnmatchedDetails const & unmatched : unmatchedDetails )
			{
				ImGui::Text( fmt::format( "Component Id Name: Unhandled Type Id: {}", unmatched.Id ).c_str() );
			}

			ImGui::EndChild();
		}
		ImGui::End();
	}

	void EditorInspector::RegisterComponentDetailsInternal( entt::type_info componentTypeInfo, std::unique_ptr<ComponentDetails> && detailsClass )
	{
		m_ComponentDetailsViewDelegates.emplace( componentTypeInfo.hash(), ComponentDetailsDelegatePair { componentTypeInfo , m_ComponentDetailsViewDelegates.size(), std::move( detailsClass ) } );
	}

	void EditorInspector::RegisterTagDetailsInternal( entt::type_info tagTypeInfo, std::unique_ptr<TagDetails> && detailsClass )
	{
		m_TagDetailsViewDelegates.emplace( tagTypeInfo.hash(), TagDelegatePair { tagTypeInfo , std::move( detailsClass ) } );
	}

}