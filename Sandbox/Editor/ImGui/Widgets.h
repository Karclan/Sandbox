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
		static void MapContainerDisplay( std::map<key, value> const & map, int & selectedIndexVar, const char * KeyName, const char * ValueName = ""  )
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
	};
}
