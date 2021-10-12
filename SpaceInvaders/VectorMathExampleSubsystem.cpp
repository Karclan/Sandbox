#include "VectorMathExampleSubsystem.h"

#include "Core/CoreRegistry.h"

#include "Graphics/Window.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/ShaderComponent.h"
#include "Graphics/TextureComponent.h"

#include "magic_enum/magic_enum.hpp"

namespace Game
{

	VectorMathExampleSubsystem::VectorMathExampleSubsystem( Core::Registry& registry)
		: Core::Subsystem(registry)
	{

	}

	void VectorMathExampleSubsystem::CreateComponents(Graphics::Window& window)
	{
		m_window = &window;
		Graphics::Renderer* renderer = window.GetRenderer();
		using namespace magic_enum::bitwise_operators;
		unsigned int basicShaderId = renderer->CreateShader("basic", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment });
		Graphics::ShaderComponent basicShaderComp{ basicShaderId };
		auto containerTextureId = renderer->CreateTexture("container.jpg");
		Graphics::TextureComponent containerTextureComp{ { containerTextureId } };

		//A
		{
			PointAEntity = m_registry.CreateEntity();
			Graphics::TransformComponent transform{ PointA };
			auto Entity = PointAEntity;
			m_registry.AddComponent<Graphics::TransformComponent>(Entity, transform);
			m_registry.AddComponent<Graphics::MeshComponent>(Entity, renderer->GetMesh(Graphics::PrimativeType::Cube));
			m_registry.AddComponent<Graphics::ShaderComponent>(Entity, basicShaderComp);
			m_registry.AddComponent<Graphics::TextureComponent>(Entity, containerTextureComp);
			m_registry.AddComponent<VectorMathExampleSubsystem::EntityTag>(Entity);
		}

		//B
		{
			PointBEntity = m_registry.CreateEntity();
			auto Entity = PointBEntity;
			Graphics::TransformComponent transform{ PointB };
			m_registry.AddComponent<Graphics::TransformComponent>(Entity, transform);
			m_registry.AddComponent<Graphics::MeshComponent>(Entity, renderer->GetMesh(Graphics::PrimativeType::Square));
			m_registry.AddComponent<Graphics::ShaderComponent>(Entity, basicShaderComp);
			m_registry.AddComponent<Graphics::TextureComponent>(Entity, containerTextureComp);
			m_registry.AddComponent<VectorMathExampleSubsystem::EntityTag>(Entity);
		}

		//C
		{
			PointCEntity = m_registry.CreateEntity();
			auto Entity = PointCEntity;
			Graphics::TransformComponent transform{ PointC };
			m_registry.AddComponent<Graphics::TransformComponent>(Entity, transform);
			m_registry.AddComponent<Graphics::MeshComponent>(Entity, renderer->GetMesh(Graphics::PrimativeType::Triangle));
			m_registry.AddComponent<Graphics::ShaderComponent>(Entity, basicShaderComp);
			m_registry.AddComponent<Graphics::TextureComponent>(Entity, containerTextureComp);
			m_registry.AddComponent<VectorMathExampleSubsystem::EntityTag>(Entity);
		}
	}

	void VectorMathExampleSubsystem::FixedUpdate(double deltaTime)
	{
		{
			Graphics::TransformComponent& Transform = m_registry.GetComponent<Graphics::TransformComponent>(PointAEntity);
			Transform.Position = PointA;
			glm::vec3 rotation = glm::eulerAngles( Transform.RotationQuat );
			float const& pitch = rotation.x;
			float const& roll = rotation.z;
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

	void VectorMathExampleSubsystem::Update(double deltaTime)
	{
	}

	void VectorMathExampleSubsystem::BuildWindow(double deltaTime)
	{
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
				glm::vec3 up = glm::cross(PointARight, PointAForward);
				ImGui::Text("A Forward: %f, %f, %f", PointAForward.x, PointAForward.y, PointAForward.z);
				ImGui::Text("A Right: %f, %f, %f", PointARight.x, PointARight.y, PointARight.z);
				ImGui::Text("A Up: %f, %f, %f", up.x, up.y, up.z);
				ImGui::Separator();
				auto PointAN = glm::length(PointA) != 0 ? glm::normalize(PointA) : PointA;

				glm::vec3 AB = PointB - PointA;
				glm::vec3 AC = PointC - PointA;

				m_window->DrawLine(PointA, PointA + AB, Graphics::Window::Black);
				m_window->DrawLine(PointA, PointA + AC, Graphics::Window::White);

				m_window->DrawLine(PointA, PointA + PointAForward, Graphics::Window::Blue );
				m_window->DrawLine(PointA, PointA + PointARight, Graphics::Window::Red );
				m_window->DrawLine(PointA, PointA + up, Graphics::Window::Green);

				float ADotB = glm::dot(PointARight, glm::normalize(AB));
				float ADotC = glm::dot(PointARight, glm::normalize(AC));
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

				std::string bDir(magic_enum::enum_name(BDirection));
				std::string cDir(magic_enum::enum_name(CDirection));
				ImGui::Text("BDirection : %s", bDir.c_str());
				ImGui::Text("CDirection : %s", cDir.c_str());
				ImGui::Separator();
			}
			ImGui::End();
		}
	}

}
