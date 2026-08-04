#include "VectorMathExampleSubsystem.h"

#include "Core/CoreRegistry.h"
#include "Core/ResourceManager.h"

#include "Graphics/Window.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/ShaderComponent.h"
#include "Graphics/TextureComponent.h"

#include "magic_enum/magic_enum.hpp"
#include "Utils/UnusedMacro.h"
#include "Graphics/GraphicsDraw.h"

namespace Game
{

	VectorMathExampleSubsystem::VectorMathExampleSubsystem( Core::Registry& registry)
		: Core::Subsystem(registry)
	{

	}

	void VectorMathExampleSubsystem::CreateComponents(Graphics::Window& window)
	{
		m_window = &window;
		Graphics::Renderer & renderer = window.GetRenderer();
		using namespace magic_enum::bitwise_operators;
		Graphics::ShaderComponent basicShaderComp = Core::Resources::CreateOrLoadShader("basic");// { Graphics::Shader::Type::Vertex, Graphics::Shader::Type::Fragment } );;
		
		Graphics::TextureComponent containerTextureComp = Core::Resources::CreateOrLoadTexture( {"container.jpg"} );

		//A
		{
			PointAEntity = m_registry.CreateEntity();
			Graphics::TransformComponent const transform{ PointA };
			auto  const Entity = PointAEntity;
			m_registry.AddComponent<Graphics::TransformComponent>(Entity, transform);
			m_registry.AddComponent<Graphics::MeshComponent>(Entity, Core::Resources::CreateOrLoadMesh(Graphics::PrimativeType::Cube));
			m_registry.AddComponent<Graphics::ShaderComponent>(Entity, basicShaderComp);
			m_registry.AddComponent<Graphics::TextureComponent>(Entity, containerTextureComp);
			m_registry.AddComponent<VectorMathExampleSubsystem::EntityTag>(Entity);
		}

		//B
		{
			PointBEntity = m_registry.CreateEntity();
			auto const Entity = PointBEntity;
			Graphics::TransformComponent const transform{ PointB };
			m_registry.AddComponent<Graphics::TransformComponent>(Entity, transform);
			m_registry.AddComponent<Graphics::MeshComponent>(Entity, Core::Resources::CreateOrLoadMesh(Graphics::PrimativeType::Square));
			m_registry.AddComponent<Graphics::ShaderComponent>(Entity, basicShaderComp);
			m_registry.AddComponent<Graphics::TextureComponent>(Entity, containerTextureComp);
			m_registry.AddComponent<VectorMathExampleSubsystem::EntityTag>(Entity);
		}

		//C
		{
			PointCEntity = m_registry.CreateEntity();
			auto const Entity = PointCEntity;
			Graphics::TransformComponent const transform{ PointC };
			m_registry.AddComponent<Graphics::TransformComponent>(Entity, transform);
			m_registry.AddComponent<Graphics::MeshComponent>(Entity, Core::Resources::CreateOrLoadMesh(Graphics::PrimativeType::Triangle));
			m_registry.AddComponent<Graphics::ShaderComponent>(Entity, basicShaderComp);
			m_registry.AddComponent<Graphics::TextureComponent>(Entity, containerTextureComp);
			m_registry.AddComponent<VectorMathExampleSubsystem::EntityTag>(Entity);
		}
	}

	void VectorMathExampleSubsystem::FixedUpdate(double deltaTime) noexcept
	{
		UNUSED( deltaTime );
		{
			Graphics::TransformComponent& Transform = m_registry.GetComponent<Graphics::TransformComponent>(PointAEntity);
			Transform.Position = PointA;
			glm::vec3 const rotation = glm::eulerAngles( Transform.RotationQuat );
			float const& pitch = rotation.x;
			//float const& roll = rotation.z;
			float const& yaw = rotation.y;
			PointAForward.x = glm::cos(pitch) * glm::sin(yaw);
			PointAForward.y = -glm::sin(pitch);
			PointAForward.z = -( glm::cos(pitch) * glm::cos(yaw) );

			PointAForward = glm::normalize(PointAForward);
			
			PointARight.x = glm::cos(yaw);
			PointARight.y = 0;
			PointARight.z = -glm::sin(yaw);

			PointARight = glm::normalize(PointARight);
		}

		{
			Graphics::TransformComponent& Transform = m_registry.GetComponent<Graphics::TransformComponent>(PointBEntity);
			Transform.Position = PointB;
		}

		{
			Graphics::TransformComponent& Transform = m_registry.GetComponent<Graphics::TransformComponent>(PointCEntity);
			Transform.Position = PointC;
		}

	}

	void VectorMathExampleSubsystem::Update(double deltaTime) noexcept
	{
		UNUSED( deltaTime );
	}

	void VectorMathExampleSubsystem::BuildWindow(double deltaTime)
	{
		UNUSED( deltaTime );

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Maths"))
			{
				ImGui::Checkbox("DotProd", &bShowWindow);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if (bShowWindow)
		{
			if (ImGui::Begin("Stats"))
			{
				ImGui::Text("Line ");
				ImGui::DragFloat3("Position A", &PointA.x, 0.01f);
				ImGui::DragFloat3("Position B", &PointB.x, 0.01f);
				ImGui::DragFloat3("Position C", &PointC.x, 0.01f);

				ImGui::Separator();
				glm::vec3 const up = glm::cross(PointARight, PointAForward);
				ImGui::Text("A Forward: %f, %f, %f", PointAForward.x, PointAForward.y, PointAForward.z);
				ImGui::Text("A Right: %f, %f, %f", PointARight.x, PointARight.y, PointARight.z);
				ImGui::Text("A Up: %f, %f, %f", up.x, up.y, up.z);
				ImGui::Separator();
				auto const PointAN = glm::length(PointA) != 0 ? glm::normalize(PointA) : PointA;

				glm::vec3 const AB = PointB - PointA;
				glm::vec3 const AC = PointC - PointA;

				Graphics::GraphicsDraw::DrawWorldLine( PointA, PointA + AB, Graphics::Colours::Black );
				Graphics::GraphicsDraw::DrawWorldLine( PointA, PointA + AC, Graphics::Colours::White );

				Graphics::GraphicsDraw::DrawWorldLine(PointA, PointA + PointAForward, Graphics::Colours::Blue );
				Graphics::GraphicsDraw::DrawWorldLine(PointA, PointA + PointARight, Graphics::Colours::Red );
				Graphics::GraphicsDraw::DrawWorldLine(PointA, PointA + up, Graphics::Colours::Green);

				float const ADotBRight = glm::dot(PointARight, glm::normalize(AB));
				float const ADotBUp = glm::dot( up, glm::normalize( AB ) );

				float const ADotCRight = glm::dot(PointARight, glm::normalize(AC));
				float const ADotCUp = glm::dot( up, glm::normalize( AC ) );

				if ( ADotBRight < 0)
				{
					BDirectionLR = EDirection::Left;
				}
				else
				{
					BDirectionLR = EDirection::Right;
				}

				if ( ADotBUp < 0 )
				{
					BDirectionUD = EDirection::Down;
				}
				else
				{
					BDirectionUD = EDirection::Up;
				}

				if ( ADotCRight < 0)
				{
					CDirectionLR = EDirection::Left;
				}
				else
				{
					CDirectionLR = EDirection::Right;
				}

				if ( ADotCUp < 0 )
				{
					CDirectionUD = EDirection::Down;
				}
				else
				{
					CDirectionUD = EDirection::Up;
				}

				std::string const bDirLR( magic_enum::enum_name( BDirectionLR ) );
				std::string const bDirUD( magic_enum::enum_name( BDirectionUD ) );
				std::string const cDirLR( magic_enum::enum_name( CDirectionLR ) );
				std::string const cDirUD( magic_enum::enum_name( CDirectionUD ) );

				ImGui::Text( "ADotBRight: %f", ADotBRight );
				ImGui::Text( "ADotBUp: %f", ADotBUp );
				ImGui::Text( "B_UD: %s", bDirUD.c_str() );
				ImGui::Text( "B_LR: %s", bDirLR.c_str() );

				ImGui::Separator();

				ImGui::Text( "ADotCRight: %f", ADotCRight );
				ImGui::Text( "ADotCUp: %f", ADotCUp );
				ImGui::Text( "C_UD : %s", cDirUD.c_str() );
				ImGui::Text( "C_LR : %s", cDirLR.c_str() );
				ImGui::Separator();
			}
			ImGui::End();
		}
	}

}
