#pragma once

#include "../Core/CoreRegistry.h"
#include "../Core/Subsystem.h"
#include "../Utils/EnumBitmask.h"
#include "../Imgui/imgui.h"

#include "ShaderProgram.h"
#include "TextureLoader.h"

#include "Colours.h"

#include "DataBuffer.h"
#include "VertexArrayObject.h"
#include "UniformBufferObject.h"

#include "Square.h"
#include "Cube.h"
#include "Triangle.h"

#include "RenderStats.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <array>

namespace Core
{
	class Registry;
}

namespace Graphics
{
	class Renderable;
	struct TransformComponent;
	struct TextureComponent;
	struct ShaderComponent;
	struct MeshComponent;

	class Renderer : public Core::ImGuiWindowBuilder
	{
	public:
		Renderer( Core::Registry & Registry, int width, int height );
		~Renderer() = default;

		ShaderComponent	CreateShader( const std::string & name, Utils::EnumBitmask<Shader::Type> flags );
		unsigned int CreateTexture( const std::string & textureName );

		MeshComponent GetMesh( PrimativeType type );

		void Init();// makes window

		void BuildWindow( double deltaTime );

		void Render();

		bool DrawLine( glm::vec3 A, glm::vec3 b, glm::vec4 colour = glm::vec4( 1.f ), float thickness = 1.f ) noexcept;

		void FramebufferSizeCallback( int width, int height );
		void ScreenSizeCallback( int width, int height );

		void SetUseFrameBufferObject( bool flag ) { m_useFrameBufferObject = flag; }

		bool IsUsingFrameBufferObject() const { return m_useFrameBufferObject; }

		unsigned int GetRenderedTexture() const { return m_renderedTexture; }

	private:
		void UpdateCamera();
		void UpdateTransform( TransformComponent & transform );
		void UpdateTextures( TextureComponent & textureComponent );
		void UpdateShader( ShaderComponent & shaderComponent );
		void UpdateMesh( MeshComponent & meshComponent );

		void CreateRenderBufferObject();
		void ResizeRenderTextureBuffer( float WidthToUse, float HeightToUse );

		Core::Registry & m_registry;

		TextureLoader m_textureLoader {};

		Triangle m_triangleMesh {};
		Square m_squareMesh {};
		Cube m_cubeMesh {};

		ShaderProgram * m_activeShader { nullptr };
		PrimativeType m_activeMeshType { PrimativeType::Invalid };

		size_t m_boundTexturesSize {};
		std::string m_currentImGuiTexture { "unset" };

		ShaderProgram * m_lineShader { nullptr };
		unsigned int m_lineDrawCallCount { 0 };
		std::shared_ptr<VertexBuffer> m_lineVbo { nullptr };
		std::shared_ptr<VertexArrayObject> m_lineVao { nullptr };

		static constexpr unsigned int m_maxLinesPerRender = 1000;

		struct LineVertex
		{
			glm::vec3 Position { 0.f };
			glm::vec4 Colour { Colours::White };
			float Thickness { 1.f };
		};

		using LineDrawCallArray = std::array<LineVertex, m_maxLinesPerRender>;
		std::array<LineVertex, m_maxLinesPerRender> m_lineDrawCallList; //Just a way of having stack allocated data we can mess around with.
		LineDrawCallArray::iterator m_lineDrawBegin;
		LineDrawCallArray::iterator m_lineDrawItr;

		glm::ivec2 m_frameBufferDimensons { 800, 600 };
		glm::ivec2 m_screenDimensions { 800, 600 };
		glm::ivec2 m_screenStartingPosition { 0,0 };
		glm::ivec2 m_frameBufferStartingPosition { 0,0 };

		glm::vec4 m_clearColour { 0.2f, 0.3f, 0.3f, 1.0f };

		struct CameraData
		{
			glm::mat4 ViewProjectionMatrix { 0.f };
		};

		CameraData m_cameraData {};
		std::shared_ptr<UniformBufferObject> m_cameraUBO { nullptr };

		bool m_showTextureWindow { false };

		ShaderProgram * m_screenShader { nullptr };

		unsigned int m_frameBufferObject { 0 };
		unsigned int m_renderedTexture { 0 };
		unsigned int m_renderBufferObject { 0 };

		bool m_useFrameBufferObject { true };
		bool m_firstTimeRenderingSubWindow { m_useFrameBufferObject };
		//ImVec2 m_sceneSize { static_cast< float >( m_frameBufferDimensons.x ), static_cast< float >( m_frameBufferDimensons.y ) };

		std::vector< std::shared_ptr<ShaderProgram> > m_shaderList;

		std::vector<GLuint> m_boundTextures {};
	};
}
