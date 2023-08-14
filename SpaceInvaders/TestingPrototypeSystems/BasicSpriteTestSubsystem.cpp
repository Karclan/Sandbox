#include "BasicSpriteTestSubsystem.h"

#include "Core/ResourceManager.h"

#include "Graphics/Window.h"

#include "Graphics/Renderer.h"
#include "Graphics/TransformComponent.h"
#include "Graphics/MeshComponent.h"
#include "Graphics/TextureComponent.h"
#include "Graphics/ShaderComponent.h"


#include "Imgui/imgui.h"
#include "Utils/UnusedMacro.h"
#include "Graphics/GraphicsDraw.h"

namespace Game
{
	BasicSpriteTestSubsystem::BasicSpriteTestSubsystem(Core::Registry& registryRef, Graphics::Window& window)
		: Subsystem(registryRef)
		, m_windowRef(window)
		, m_entity(Core::NullEntity)
	{
		AStart = glm::vec3(5.f, 0.f, 0.f);
		AEnd = glm::vec3(0.f, 5.f, 0.f);
		BStart = glm::vec3(-0.f, -5.f, 0.f);
		BEnd = glm::vec3(-5.f, -0.f, 0.f);
	}

	void BasicSpriteTestSubsystem::CreateComponents(Graphics::Window& windowRef)
	{
		auto & renderer = windowRef.GetRenderer();
		
		Graphics::ShaderComponent textureEnabledShaderComp = Core::Resources::CreateOrLoadShader("basic");// { Graphics::Shader::Type::Vertex, Graphics::Shader::Type::Fragment });
		//auto textureEnabledShaderId = renderer->createShader( "textureEnabled", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment } );
	   // auto rayShaderId = renderer->createShader( "ray", { Graphics::Shader::Type::Vertex , Graphics::Shader::Type::Fragment , Graphics::Shader::Type::Geometry } );
		auto containerTextureId = renderer.GetOrCreateTexture("container.jpg");

		Graphics::TextureComponent containerTextureComp{ { containerTextureId } };

		/*for( unsigned int i = 0; i < 1; ++i )
		{*/
		Graphics::TransformComponent triangleTransform;
		triangleTransform.Position = { 1.f, 1.f, 1.f };

		m_entity = Subsystem::m_registry.CreateEntity();
		Subsystem::m_registry.AddComponent<Graphics::TransformComponent>(m_entity, triangleTransform);
		Subsystem::m_registry.AddComponent<Graphics::MeshComponent>(m_entity, renderer.GetMesh(Graphics::PrimativeType::Cube));
		Subsystem::m_registry.AddComponent<Graphics::TextureComponent>(m_entity, containerTextureComp);
		Subsystem::m_registry.AddComponent<Graphics::ShaderComponent>(m_entity, textureEnabledShaderComp);
		//}

	}

	void BasicSpriteTestSubsystem::FixedUpdate(double deltaTime) noexcept
	{
		UNUSED(deltaTime);
	}

	void BasicSpriteTestSubsystem::Update(double deltaTime) noexcept
	{
		UNUSED(deltaTime);
		Graphics::GraphicsDraw::DrawLine(AStart, AEnd, { 1.f, 1.f, 0.f, 1.f }, 1.f);
		Graphics::GraphicsDraw::DrawLine(BStart, BEnd, { 1.f, 1.f, 0.f, 1.f }, 1.f);

		//XYZ widgets
		//Should really draw these on bottom left of screen
		Graphics::GraphicsDraw::DrawLine({ 0.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }, 1.f);
		Graphics::GraphicsDraw::DrawLine({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 1.f, 0.f, 1.f }, 1.f);
		Graphics::GraphicsDraw::DrawLine({ 0.f, 0.f, 0.f }, { 0.f, 0.f, -1.f }, { 0.f, 0.f, 1.f, 1.f }, 1.f);
	}

	void BasicSpriteTestSubsystem::BuildWindow(double deltaTime)
	{
		UNUSED(deltaTime);
		auto& movementStruct = Subsystem::m_registry.GetComponent<Graphics::TransformComponent>(m_entity);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Test"))
			{
				ImGui::Checkbox("Line", &m_showWindow);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (m_showWindow)
		{
			if (ImGui::Begin("Stats"))
			{
				ImGui::Text("Line ");
				ImGui::DragFloat2("Position A Start", &AStart.x);
				ImGui::DragFloat2("Position A End", &AEnd.x);
				ImGui::DragFloat2("Position B Start", &BStart.x);
				ImGui::DragFloat2("Position B End", &BEnd.x);

				if (!ImGui::IsAnyMouseDown())
				{
					m_bobsEulers = glm::degrees(glm::eulerAngles(movementStruct.RotationQuat));
				}

				ImGui::DragFloat3("Bob", &movementStruct.Position.x);
				if (ImGui::DragFloat3("Bob's Rotation", &m_bobsEulers.x))
				{
					movementStruct.RotationQuat = glm::radians(m_bobsEulers);
				}

				ImGui::Separator();
			}
			ImGui::End();
		}
	}
}