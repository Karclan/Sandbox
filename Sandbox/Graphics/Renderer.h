#pragma once

#include "ShaderProgram.h"

#include "TextureLoader.h"

#include "Square.h"
#include "Cube.h"
#include "Triangle.h"

#include "../Utils/EnumBitmask.h"

#include <entt/entt.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <array>

#include "../Imgui/imgui.h"

namespace Graphics
{
    class Renderable;
    struct TransformComponent;
    struct TextureComponent;
    struct ShaderComponent;
    struct MeshComponent;

    class Renderer
    {
    public:
        Renderer(entt::registry& Registry);
        ~Renderer();

        unsigned int CreateShader(const std::string& name, Utils::EnumBitmask<Shader::Type> flags);
        unsigned int CreateTexture(const std::string& textureName);
        unsigned int CreateCamera(const std::string& cameraName);

        MeshComponent GetMesh(PrimativeType type);

        void Init();// makes window

        void SetActiveCamera(unsigned int cameraId);

        void BuildImGui(float deltaTime);

        void Render();

        bool DrawLine(glm::vec3 A, glm::vec3 b, glm::vec4 colour = glm::vec4(1.f), float thickness = 1.f);

        void FramebufferSizeCallback(int width, int height);

        void SetUseFrameBufferObject(bool flag) { m_useFrameBufferObject = flag; }

        bool IsUsingFrameBufferObject() const { return m_useFrameBufferObject; }

        unsigned int GetRenderedTexture() const { return m_renderedTexture; }

    private:
        void UpdateCamera();
        void UpdateTransform(TransformComponent& transform);
        void UpdateTextures(TextureComponent& textureComponent);
        void UpdateShader(ShaderComponent& shaderComponent);
        void UpdateMesh(MeshComponent& meshComponent);

        void CreateRenderBufferObject();

        static entt::entity m_nullCameraEntity;

        entt::registry& m_registry;
        entt::entity& m_activeCamera;

        TextureLoader m_textureLoader{};

        Triangle m_triangleMesh{};
        Square m_squareMesh{};
        Cube m_cubeMesh{};

        ShaderProgram* m_activeShader{ nullptr };
        PrimativeType m_activeMeshType{ PrimativeType::Invalid };

        size_t m_boundTexturesSize{};
        std::string m_currentImGuiTexture;

        ShaderProgram* m_lineShader{ nullptr };
        unsigned int m_lineDrawCalls{ 0 };
        unsigned int m_lineVbo{ 0 };
        unsigned int m_lineVao{ 0 };
        unsigned int m_lineEbo{ 0 };

        static constexpr unsigned int m_maxLinesPerRender = 1000;

        struct LineDrawCall
        {
            glm::vec3 PointA{ 0.f };
            glm::vec3 PointB{ 0.f };
            glm::vec4 m_colour{ 1.f };
            float m_lineThickness{ 1.f };
        };

        std::array<LineDrawCall, m_maxLinesPerRender> m_lineDrawCallList;

        int m_width{ 800 };
        int m_height{ 600 };
        bool m_updateCamera{ true };

        bool m_showCameraControls{ false };
        bool m_showCameraDebug { false };
        bool m_liveCameraUpdate{ true };

        bool m_showTextureWindow{ false };

		ShaderProgram* m_screenShader{ nullptr };

        unsigned int m_frameBufferObject{ 0 };
        unsigned int m_renderedTexture{ 0 };
        unsigned int m_renderBufferObject{ 0 };

		bool m_useFrameBufferObject{ true };
		ImVec2 m_sceneSize{ 800, 600 };

        std::vector< std::unique_ptr<ShaderProgram> > m_shaderList;
        std::vector<entt::entity> m_cameraEntities {};
        std::vector<GLuint> m_boundTextures {};

    };
}
