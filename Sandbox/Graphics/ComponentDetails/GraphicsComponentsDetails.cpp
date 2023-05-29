#include "GraphicsComponentsDetails.h"

#include "../magic_enum/magic_enum.hpp"
#include "../Utils/format.h"

#include "../TransformComponent.h"
#include "../MeshComponent.h"
#include "../CameraComponent.h"
#include "../ShaderComponent.h"
#include "../TextureComponent.h"

#include "imgui.h"
#include "glm/glm.hpp"

#include <array>
#include <string>

namespace Editor
{
	//work on getting tags viewable in editor
	//work on getting camera types switchable
	void CameraComponentDetails::BuildDetailsInternal( Graphics::CameraComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Camera" ) )
		{
			//ImGui::Text( fmt::format( "{} - Name", inComponent.m_name ).c_str() );

			//Dropdown
			{
				static size_t index = 0;
				static std::string previewString = "Unknown type";

				auto constexpr projectionTypeNames = magic_enum::enum_names<Graphics::ProjectionType>();
				auto constexpr projectionTypeCount = magic_enum::enum_count<Graphics::ProjectionType>();

				auto possibleIndex = magic_enum::enum_index<Graphics::ProjectionType>( inComponent.m_projectionType );
				if ( possibleIndex.has_value() )
				{
					index = possibleIndex.value();
					previewString = projectionTypeNames[ index ];
				}

				if ( ImGui::BeginCombo( "Projection Type", previewString.c_str() ) )
				{
					for ( size_t i = 0; i < projectionTypeCount; ++i )
					{
						bool const is_selected = ( index == i );
						if ( ImGui::Selectable( std::string( projectionTypeNames[ i ] ).c_str(), is_selected ) )
						{
							index = i;
							inComponent.m_projectionType = magic_enum::enum_value< Graphics::ProjectionType >( static_cast< int >( i ) );
						}

						if ( is_selected )  // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}

			ImGui::InputDouble( "Camera Speed", &inComponent.m_cameraSpeed );
			ImGui::DragFloat3( "Direction", &inComponent.m_direction.x );
			ImGui::DragFloat3( "Target", &inComponent.m_target.x );
			ImGui::DragFloat3( "Up", &inComponent.m_up.x );

			ImGui::Checkbox( "Dirty Projection Matrix", &inComponent.m_dirtyProjectionMatrix );
			ImGui::Checkbox( "Dirty View Matrix", &inComponent.m_dirtyViewMatrix );

			ImGui::DragFloat( "Far Plane", &inComponent.m_farPlane );
			ImGui::DragFloat( "Near Plane", &inComponent.m_nearPlane );
			ImGui::DragFloat( "FOV", &inComponent.m_fov );

			ImGui::Checkbox( "Orbit", &inComponent.m_orbit );

			//custom widgets needed
			//inComponent.m_projectionMatrix;
			//inComponent.m_viewMatrix;

			ImGui::DragFloat( "View Port Height", &inComponent.m_viewPortHeight );
			ImGui::DragFloat( "View Port Width", &inComponent.m_viewPortWidth );

			ImGui::DragFloat( "Ortho Bottom", &inComponent.m_orthoBottom );
			ImGui::DragFloat( "Ortho Top", &inComponent.m_orthoTop );
			ImGui::DragFloat( "Ortho Left", &inComponent.m_orthoLeft );
			ImGui::DragFloat( "Ortho Right", &inComponent.m_orthoRight );
		}
	}

	void TransformComponentDetails::BuildDetailsInternal( Graphics::TransformComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Transform" ) )
		{
			ImGui::DragFloat3( "Position", &inComponent.Position.x );
			ImGui::DragFloat3( "Scale", &inComponent.Scale.x );

			glm::vec3 eulerRotation = glm::eulerAngles( inComponent.RotationQuat );

			ImGui::DragFloat3( "Rotation", &eulerRotation.x );

			inComponent.RotationQuat = glm::quat( eulerRotation );
		}
	}

	void MeshComponentDetails::BuildDetailsInternal( Graphics::MeshComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Mesh" ) )
		{
			ImGui::Text( fmt::format( "{}: Mesh Id", inComponent.MeshId ).c_str() );
			ImGui::Text( fmt::format( "{}: Primitive Type", magic_enum::enum_name<Graphics::PrimativeType>( inComponent.type ) ).c_str() );
		}
	}

	void ShaderComponentDetails::BuildDetailsInternal( Graphics::ShaderComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Shader" ) )
		{
			ImGui::Text( fmt::format( "{}: Shader Id", inComponent.ShaderId ).c_str() );
		}
	}

	void TextureComponentDetails::BuildDetailsInternal( Graphics::TextureComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Textures" ) )
		{
			size_t textureCount = inComponent.TextureList.size();
			for ( size_t i = 0; i < textureCount; ++i )
			{
				ImGui::PushID( static_cast< int >( i ) );
				ImGui::Text( fmt::format( "{} TextureId", inComponent.TextureList[ i ] ).c_str() );
				ImGui::PopID();
			}
		}
	}

}
