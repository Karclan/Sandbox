#include "VectorMathExampleSubsystem.h"

#include "Graphics/Window.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/ShaderComponent.h"
#include "Graphics/TextureComponent.h"

namespace Game
{

	VectorMathExampleSubsystem::VectorMathExampleSubsystem(entt::registry& registry)
		: Subsystem(registry)
		, ImGuiWindowBuilder(registry)
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
			PointAEntity = Subsystem::m_registry.create();
			Graphics::TransformComponent transform{ PointA };
			auto Entity = PointAEntity;
			Subsystem::m_registry.assign<Graphics::TransformComponent>(Entity, transform);
			Subsystem::m_registry.assign<Graphics::MeshComponent>(Entity, renderer->GetMesh(Graphics::PrimativeType::Cube));
			Subsystem::m_registry.assign<Graphics::ShaderComponent>(Entity, basicShaderComp);
			Subsystem::m_registry.assign<Graphics::TextureComponent>(Entity, containerTextureComp);
			Subsystem::m_registry.assign<VectorMathExampleSubsystem::EntityTag>(Entity);
		}

		//B
		{
			PointBEntity = Subsystem::m_registry.create();
			auto Entity = PointBEntity;
			Graphics::TransformComponent transform{ PointB };
			Subsystem::m_registry.assign<Graphics::TransformComponent>(Entity, transform);
			Subsystem::m_registry.assign<Graphics::MeshComponent>(Entity, renderer->GetMesh(Graphics::PrimativeType::Square));
			Subsystem::m_registry.assign<Graphics::ShaderComponent>(Entity, basicShaderComp);
			Subsystem::m_registry.assign<Graphics::TextureComponent>(Entity, containerTextureComp);
			Subsystem::m_registry.assign<VectorMathExampleSubsystem::EntityTag>(Entity);
		}

		//C
		{
			PointCEntity = Subsystem::m_registry.create();
			auto Entity = PointCEntity;
			Graphics::TransformComponent transform{ PointC };
			Subsystem::m_registry.assign<Graphics::TransformComponent>(Entity, transform);
			Subsystem::m_registry.assign<Graphics::MeshComponent>(Entity, renderer->GetMesh(Graphics::PrimativeType::Triangle));
			Subsystem::m_registry.assign<Graphics::ShaderComponent>(Entity, basicShaderComp);
			Subsystem::m_registry.assign<Graphics::TextureComponent>(Entity, containerTextureComp);
			Subsystem::m_registry.assign<VectorMathExampleSubsystem::EntityTag>(Entity);
		}
	}

	void VectorMathExampleSubsystem::FixedUpdate(double deltaTime)
	{
		{
			Graphics::TransformComponent& Transform = Subsystem::m_registry.get<Graphics::TransformComponent>(PointAEntity);
			Transform.Position = PointA;
			float const& pitch = Transform.Rotation.x;
			float const& roll = Transform.Rotation.z;
			float const& yaw = Transform.Rotation.y;
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
			Graphics::TransformComponent& Transform = Subsystem::m_registry.get<Graphics::TransformComponent>(PointBEntity);
			Transform.Position = PointB;
		}

		{
			Graphics::TransformComponent& Transform = Subsystem::m_registry.get<Graphics::TransformComponent>(PointCEntity);
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
