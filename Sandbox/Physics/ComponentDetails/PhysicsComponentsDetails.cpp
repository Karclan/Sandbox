#include "PhysicsComponentsDetails.h"

#include "../CollisionComponent.h"

#include "../Utils/format.h"
#include "../Utils/customformatters.h"

#include "../magic_enum/magic_enum.hpp"

#include "imgui.h"

namespace Editor
{
	constexpr float FloatDragSpeed = 0.01f;

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
			ImGui::PushID( "##Details" );
			ImGui::Text( fmt::format( "Bottom Right: {}", inComponent.bottomRight ).c_str() );
			ImGui::Text( fmt::format( "Top Left: {}", inComponent.topLeft ).c_str() );
			ImGui::DragFloat( "Height", &inComponent.height, FloatDragSpeed );
			ImGui::DragFloat( "Width", &inComponent.width, FloatDragSpeed );
			ImGui::DragFloat2( "Offset", &inComponent.offset.x, FloatDragSpeed );
			ImGui::Checkbox( "Recalculate Extents", &inComponent.recalculateExtents );
			ImGui::PopID();
		}
	}

	void CircleExtentsDetails::BuildDetailsInternal( Physics::CircleExtents & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Circle Extents" ) )
		{
			ImGui::PushID( "##Details" );
			ImGui::DragFloat( "Radius", &inComponent.radius, FloatDragSpeed );
			ImGui::PopID();
		}
	}

	void SquareExtentsDebugRenderComponentDetails::BuildDetailsInternal( Physics::SquareExtentsDebugRenderComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Square Debug Render Extents" ) )
		{
			ImGui::PushID( "##Details" );
			ImGui::Text( fmt::format( "Bottom Right: {}", inComponent.cachedBottomRight ).c_str() );
			ImGui::Text( fmt::format( "Top Left: {}", inComponent.cachedTopLeft ).c_str() );
			ImGui::Text( fmt::format( "Height: {}", inComponent.cachedHeight ).c_str() );
			ImGui::Text( fmt::format( "Width: {}", inComponent.cachedWidth ).c_str() );
			ImGui::Text( fmt::format( "Offset: {}", inComponent.cachedOffset ).c_str() );
			ImGui::Checkbox( "Recalculate Mesh", &inComponent.recalculateDebugMesh );
			ImGui::PopID();
		}
	}

	void CircleExtentsDebugRenderComponentDetails::BuildDetailsInternal( Physics::CircleExtentsDebugRenderComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Circle Debug Render Extents" ) )
		{
			ImGui::PushID( "##Details" );
			ImGui::DragInt( "Point Count", &inComponent.PointCount, 1.f, 0, 360 );
			ImGui::Text( fmt::format( "Cached Radius: {}", inComponent.CachedRadius).c_str() );
			ImGui::PopID();
		}
	}

}
