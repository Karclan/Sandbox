#pragma once

#include "../../Utils/format.h"

#include "imgui.h"
#include "../Imgui/misc/cpp/imgui_stdlib.h"
#include "../Utils/customformatters.h"

#include <map>
#include <vector>
#include <string>

namespace Editor
{
	class Widgets
	{
	public:
		template<typename key, typename value>
		static void MapContainerDisplay( std::map<key, value> const & map, int & selectedIndexVar, const char * KeyName, const char * ValueName = "" )
		{
			ImGui::PushID( fmt::format( "{}MapColums", KeyName ).c_str() );
			ImGui::Columns( 2, fmt::format( "{}MapColums", KeyName ).c_str() );
			ImGui::Separator();
			ImGui::Text( KeyName ); ImGui::NextColumn();
			ImGui::Text( ValueName ); ImGui::NextColumn();
			ImGui::Separator();
			size_t const mapSize = map.size();
			std::vector<std::string> keys;
			std::vector<std::string> values;
			for ( auto it = map.begin(); it != map.end(); ++it )
			{
				keys.push_back( fmt::format( "{}", it->first ) );
				values.push_back( fmt::format( "{}", it->second ) );
			}

			for ( size_t i = 0; i < mapSize; ++i )
			{
				if ( ImGui::Selectable( keys[ i ].c_str(), selectedIndexVar == static_cast< int >( i ), ImGuiSelectableFlags_SpanAllColumns ) )
				{
					if ( selectedIndexVar == i )
					{
						selectedIndexVar = -1;
					}
					else
					{
						selectedIndexVar = static_cast< int >( i );
					}
				}
				ImGui::NextColumn();
				ImGui::Text( values[ i ].c_str() ); ImGui::NextColumn();
			}
			ImGui::Columns( 1 );
			ImGui::PopID();
		}

		static void HelpMarker( const char * desc )
		{
			ImGui::TextDisabled( "(?)" );
			if ( ImGui::IsItemHovered( ImGuiHoveredFlags_DelayShort ) && ImGui::BeginTooltip() )
			{
				ImGui::PushTextWrapPos( ImGui::GetFontSize() * 35.0f );
				ImGui::TextUnformatted( desc );
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		//static bool ComboHelper( const char * Title, std::string & comboLabel, auto & container, size_t containerSize, const char * labelFormat, int & selectionIndex )
		//{
		//	bool selectionUpdated = false;
		//	if ( ImGui::BeginCombo( Title, comboLabel.c_str() ) )
		//	{
		//		for ( size_t i = 0; i < containerSize; i++ )
		//		{
		//			auto element = container[ i ];
		//			std::string const label = fmt::format( labelFormat, element );
		//			bool const is_selected = ( selectionIndex == i );
		//			if ( ImGui::Selectable( label.c_str(), is_selected ) )
		//			{
		//				selectionIndex = static_cast<int>(i);
		//				selectionUpdated = true;
		//			}
		//			if ( is_selected )  // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
		//			{
		//				ImGui::SetItemDefaultFocus();
		//			}
		//		}
		//		ImGui::EndCombo();
		//	}
		//	return selectionUpdated;
		//}

		//if ( ImGui::BeginCombo( "Entities", comboLabel.c_str() ) )
		//{
		//	for ( size_t i = 0; i < entityListSize; i++ )
		//	{
		//		Core::Entity entity = entityList[ i ];
		//		std::string const label = generateNameLabel( entity );
		//		bool const is_selected = ( entityListIndex == i );
		//		if ( ImGui::Selectable( label.c_str(), is_selected ) )
		//		{
		//			entityListIndex = i;
		//			m_selectedEntity = entityList[ entityListIndex ];
		//			setContextEntity();
		//		}
		//		if ( is_selected )  // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
		//		{
		//			ImGui::SetItemDefaultFocus();
		//		}
		//	}
		//	ImGui::EndCombo();
		//}
	};

}
