#include "PhysicsComponentsDetails.h"

#include "../magic_enum/magic_enum.hpp"
#include "../Utils/format.h"

#include "../CollisionComponent.h"

#include "imgui.h"

namespace Editor
{
	void CollisionComponentDetails::BuildDetailsInternal( Physics::CollisionComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Collision Component" ) )
		{
			ImGui::Checkbox( "Is Colliding", &inComponent.isColliding );

			//Dropdown
			{
				static size_t index = 0;
				static std::string previewString = "Unknown type";

				auto constexpr & typeNames = magic_enum::enum_names<Physics::CollisionComponent::ShapeType>();
				auto constexpr typeCount = magic_enum::enum_count<Physics::CollisionComponent::ShapeType>();

				auto possibleIndex = magic_enum::enum_index<Physics::CollisionComponent::ShapeType>( inComponent.Shape );
				if ( possibleIndex.has_value() )
				{
					index = possibleIndex.value();
					previewString = typeNames[ index ];
				}

				if ( ImGui::BeginCombo( "Collision Shape", previewString.c_str() ) )
				{
					for ( size_t i = 0; i < typeCount; ++i )
					{
						bool const is_selected = ( index == i );
						if ( ImGui::Selectable( std::string( typeNames[ i ] ).c_str(), is_selected ) )
						{
							index = i;
						}

						if ( is_selected )  // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}
		}
	}

	void SquareExtentsDetails::BuildDetailsInternal( Physics::SquareExtents & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Square Extents" ) )
		{
			ImGui::DragFloat2( "Bottom Right", &inComponent.bottomRight.x );
			ImGui::DragFloat2( "Top Left", &inComponent.topLeft.x );
			ImGui::InputDouble( "Height", &inComponent.height );
			ImGui::InputDouble( "Width", &inComponent.width );
			ImGui::Checkbox( "Recalculate Extents", &inComponent.recalculateExtents );
		}
	}

	void CircleExtentsDetails::BuildDetailsInternal( Physics::CircleExtents & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Circle Extents" ) )
		{
			ImGui::InputDouble( "Radius", &inComponent.radius);
		}
	}

	void SquareExtentsDebugRenderComponentDetails::BuildDetailsInternal( Physics::SquareExtentsDebugRenderComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Circle Extents" ) )
		{
			ImGui::DragFloat2( "Bottom Right", &inComponent.cachedBottomRight.x );
			ImGui::DragFloat2( "Top Left", &inComponent.cachedTopLeft.x );
			ImGui::InputDouble( "Height", &inComponent.cachedHeight );
			ImGui::InputDouble( "Width", &inComponent.cachedWidth );
			ImGui::Checkbox( "Recalculate Mesh", &inComponent.recalculateDebugMesh );
		}
	}

}
