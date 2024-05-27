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
			auto const Entity = PointAEntity;
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

				float const ADotB = glm::dot(PointARight, glm::normalize(AB));
				float const ADotC = glm::dot(PointARight, glm::normalize(AC));
				ImGui::Text("ADotB: %f", ADotB);
				ImGui::Text("ADotC: %f", ADotC);

				if (ADotB < 0)
				{
					BDirection = EDirection::Left;
				}
				else if (ADotB == 0)
				{
					BDirection = EDirection::Down;
				}
				else
				{
					BDirection = EDirection::Right;
				}

				if (ADotC < 0)
				{
					CDirection = EDirection::Left;
				}
				else if (ADotC == 0)
				{
					CDirection = EDirection::Down;
				}
				else
				{
					CDirection = EDirection::Right;
				}

				std::string const bDir(magic_enum::enum_name(BDirection));
				std::string const cDir(magic_enum::enum_name(CDirection));
				ImGui::Text("BDirection : %s", bDir.c_str());
				ImGui::Text("CDirection : %s", cDir.c_str());
				ImGui::Separator();
			}
			ImGui::End();
		}
	}

}
