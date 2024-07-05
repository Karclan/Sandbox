#include "GraphicsComponentsDetails.h"

#include "../magic_enum/magic_enum.hpp"
#include "../Utils/format.h"

#include "../Editor/ImGui/Widgets.h"

#include "../TransformComponent.h"
#include "../MeshComponent.h"
#include "../CameraComponent.h"
#include "../ShaderComponent.h"
#include "../TextureComponent.h"

#include "imgui.h"
#include "glm/glm.hpp"
#include <glm/gtx/string_cast.hpp>

#include <array>
#include <string>
#include <numeric>

#include "../magic_enum/magic_enum_format.hpp"

namespace Editor
{
	constexpr float FloatDragSpeed = 0.01f;

	//work on getting tags viewable in editor
	//work on getting camera types switchable
	void CameraComponentDetails::BuildDetailsInternal( Graphics::CameraComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Camera" ) )
		{
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

			ImGui::InputDouble( "Camera Speed", &inComponent.m_cameraSpeed, FloatDragSpeed );
			//ImGui::DragFloat3( "Direction", &inComponent.m_direction.x );
			ImGui::DragFloat3( "Target", &inComponent.m_target.x, FloatDragSpeed );
			ImGui::DragFloat3( "Up", &inComponent.m_up.x, FloatDragSpeed );

			inComponent.m_dirtyProjectionMatrix = true;
			inComponent.m_dirtyViewMatrix = true;
			ImGui::Checkbox( "Dirty Projection Matrix", &inComponent.m_dirtyProjectionMatrix );
			ImGui::Checkbox( "Dirty View Matrix", &inComponent.m_dirtyViewMatrix );

			ImGui::DragFloat( "Far Plane", &inComponent.m_farPlane, FloatDragSpeed );
			ImGui::DragFloat( "Near Plane", &inComponent.m_nearPlane, FloatDragSpeed );
			ImGui::DragFloat( "FOV", &inComponent.m_fov );

			ImGui::Checkbox( "Orbit", &inComponent.m_orbit );

			ImGui::DragFloat( "View Port Height", &inComponent.m_viewPortHeight );
			ImGui::DragFloat( "View Port Width", &inComponent.m_viewPortWidth );

			ImGui::DragFloat( "Ortho Aspect Zoom", &inComponent.m_orthoAspectScale, FloatDragSpeed );

			ImGui::Text( fmt::format( "Projection Matrix \n{}", inComponent.m_projectionMatrix ).c_str() );
			ImGui::Text( fmt::format( "View Matrix \n{}", inComponent.m_viewMatrix ).c_str() );
		}
	}

	void TransformComponentDetails::BuildDetailsInternal( Graphics::TransformComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Transform" ) )
		{
			ImGui::DragFloat3( "Position", &inComponent.Position.x, FloatDragSpeed );
			ImGui::DragFloat3( "Scale", &inComponent.Scale.x, FloatDragSpeed );

			glm::vec3 eulerRotation = glm::degrees( glm::eulerAngles( inComponent.RotationQuat ) );

			if ( ImGui::DragFloat3( "Rotation", &eulerRotation.x, FloatDragSpeed ) )
			{
				inComponent.RotationQuat = glm::quat( glm::radians( eulerRotation ) );
			}
		}
	}

	void MeshComponentDetails::BuildDetailsInternal( Graphics::MeshComponent & inComponent ) const
	{
		if ( ImGui::CollapsingHeader( "Mesh" ) )
		{
			ImGui::Text( fmt::format( "{}: Mesh Id", inComponent.MeshId ).c_str() );
			ImGui::Text( fmt::format( "{}: Primitive Type", magic_enum::enum_name<Graphics::PrimativeType>( inComponent.Type ) ).c_str() );
		}
	}

	void ShaderComponentDetails::BuildDetailsInternal( Graphics::ShaderComponent & inComponent ) const
	{
		if ( std::shared_ptr<Graphics::IShaderHandle const> const shaderHandle = inComponent.ShaderHandle.lock() )
		{
			std::string const & name = shaderHandle->GetShaderName();
			unsigned int const id = shaderHandle->GetShaderId();

			if ( ImGui::CollapsingHeader( fmt::format( "Shader: {} Id: {}", name, id ).c_str() ) )
			{
				Utils::EnumBitmask<Graphics::Shader::Type> const & shaderTypes = shaderHandle->GetShaderTypes();

				auto const & shaderTypesEntries = magic_enum::enum_values<Graphics::Shader::Type>();
				for ( auto const & type : shaderTypesEntries )
				{
					if ( type == Graphics::Shader::Type::Invalid || type == Graphics::Shader::Type::Total )
					{
						continue;
					}

					if ( shaderTypes & type )
					{
						std::string name( magic_enum::enum_name<Graphics::Shader::Type>( type ) );
						if ( ImGui::CollapsingHeader( name.c_str() ) )
						{
							std::string const & source = shaderHandle->GetShaderSource( type );
							ImGui::Text( source.c_str() );
							ImGui::Separator();
							std::string const & path = shaderHandle->GetShaderPath( type );
							ImGui::Text( path.c_str() );
						}

					}
				}
				ImGui::Separator();

				auto const & uniforms = shaderHandle->GetUniforms();
				auto const & bools = shaderHandle->GetBoolUniforms();
				auto const & ints = shaderHandle->GetIntUniforms();
				auto const & floats = shaderHandle->GetFloatUniforms();
				auto const & gluints = shaderHandle->GetGLuintUniforms();
				auto const & vec2s = shaderHandle->GetVec2Uniforms();
				auto const & vec3s = shaderHandle->GetVec3Uniforms();
				auto const & vec4s = shaderHandle->GetVec4Uniforms();
				auto const & mat3s = shaderHandle->GetMat3Uniforms();
				auto const & mat4s = shaderHandle->GetMat4Uniforms();
				//less than ideal but not sure on best way to have indexes like this.
				static int uniformIndex = -1;
				static int boolIndex = -1;
				static int intIndex = -1;
				static int floatIndex = -1;
				static int gluintIndex = -1;
				static int vec2Index = -1;
				static int vec3Index = -1;
				static int vec4Index = -1;
				static int mat3Index = -1;
				static int mat4Index = -1;
				Widgets::MapContainerDisplay( uniforms, uniformIndex, "Uniforms", "Location" );
				Widgets::MapContainerDisplay( bools, boolIndex, "Bools" );
				Widgets::MapContainerDisplay( ints, intIndex, "Ints" );
				Widgets::MapContainerDisplay( floats, floatIndex, "Floats" );
				Widgets::MapContainerDisplay( gluints, gluintIndex, "GLuints" );
				Widgets::MapContainerDisplay( vec2s, vec2Index, "Vec2s" );
				Widgets::MapContainerDisplay( vec3s, vec3Index, "Vec3s" );
				Widgets::MapContainerDisplay( vec4s, vec4Index, "Vec4s" );
				Widgets::MapContainerDisplay( mat3s, mat3Index, "Mat3s" );
				Widgets::MapContainerDisplay( mat4s, mat4Index, "Mat4s" );
			}
		}
		else
		{
			if ( ImGui::CollapsingHeader( fmt::format( "Shader: <unknown> Id: {} ", inComponent.ShaderId ).c_str() ) )
			{
				ImGui::Text( "ShaderHandle failed to lock! Shader is somehow dead." );
			}
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
