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
#include "FrameBuffer.h"
#include "ShaderResourceHandler.h"
#include "TextureResourceHandler.h"
#include "MeshResourceHandler.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <map>
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

		void GLDebugOutput( GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char * message, const void * userParam );

		/*ShaderComponent	GetOrCreateShader( std::string const & name, Utils::EnumBitmask<Shader::Type> flags );*/
		/*unsigned int GetOrCreateTexture( std::string const & textureName );*/

		//MeshComponent GetMesh( PrimativeType type );

		void Init();// makes window

		void BuildWindow( double deltaTime );

		void Render();

		bool DrawScreenLine( glm::vec2 A, glm::vec3 b, glm::vec4 colour = glm::vec4( 1.f ), float thickness = 1.f ) noexcept;
		bool DrawWorldLine( glm::vec3 A, glm::vec3 b, glm::vec4 colour = glm::vec4( 1.f ), float thickness = 1.f ) noexcept;
		bool DrawCircle( glm::vec3 pos, float radius, glm::vec4 colour = glm::vec4( 1.f ), int pointCount = 3.f ) noexcept;
		bool DrawWorldPoint( glm::vec3 pos, float size, glm::vec4 colour = Colours::White ) noexcept;
		bool DrawScreenPoint( glm::vec2 pos, float size, glm::vec4 colour = Colours::White ) noexcept;

		void FramebufferSizeCallback( int width, int height );
		void ScreenSizeCallback( int width, int height );

		void SetUseFrameBufferObject( bool flag ) { m_useFrameBufferObject = flag; }

		bool IsUsingFrameBufferObject() const { return m_useFrameBufferObject; }

	private:
		void UpdateCamera();
		void UpdateTransform( TransformComponent & transform );
		void UpdateTextures( TextureComponent & textureComponent );
		void UpdateShader( ShaderComponent & shaderComponent );
		void UpdateMesh( MeshComponent & meshComponent );

		void ResizeRenderTextureBuffer( float WidthToUse, float HeightToUse );

		Core::Registry & m_registry;

		//TextureLoader m_textureLoader {};

		SB::Shared<Mesh> m_triangleMesh {};
		SB::Shared<Mesh> m_squareMesh {};
		SB::Shared<Mesh> m_cubeMesh {};

		SB::Shared<ShaderProgram> m_activeShader { nullptr };
		PrimativeType m_activeMeshType { PrimativeType::Invalid };

		size_t m_boundTexturesSize {};
		std::string m_currentImGuiTexture { "unset" };


		//////////////////////////////////////////////////////////////////////////
		//Screen Line Shader
		//////////////////////////////////////////////////////////////////////////

		SB::Shared<ShaderProgram> m_screenLineShader { nullptr };
		unsigned int m_screenLineDrawCallCount { 0 };
		std::shared_ptr<VertexBuffer> m_screenLineVbo { nullptr };
		std::shared_ptr<VertexArrayObject> m_screenLineVao { nullptr };

		static constexpr unsigned int m_maxScreenLinesPerRender = 2000;

		struct ScreenLineVertex
		{
			glm::vec2 Position { 0.f };
			glm::vec4 Colour { Colours::White };
			float Thickness { 1.f };
		};

		using ScreenLineDrawCallArray = std::array<ScreenLineVertex, m_maxScreenLinesPerRender>;
		std::array<ScreenLineVertex, m_maxScreenLinesPerRender> m_screenLineDrawCallList; //Just a way of having stack allocated data we can mess around with.
		ScreenLineDrawCallArray::iterator m_screenLineDrawBegin;
		ScreenLineDrawCallArray::iterator m_screenLineDrawItr;

		//////////////////////////////////////////////////////////////////////////
		//World Line Shader
		//////////////////////////////////////////////////////////////////////////

		SB::Shared<ShaderProgram> m_worldLineShader { nullptr };
		unsigned int m_worldLineDrawCallCount { 0 };
		std::shared_ptr<VertexBuffer> m_worldLineVbo { nullptr };
		std::shared_ptr<VertexArrayObject> m_worldLineVao { nullptr };

		static constexpr unsigned int m_maxWorldLinesPerRender = 2000;

		struct WorldLineVertex
		{
			glm::vec3 Position { 0.f };
			glm::vec4 Colour { Colours::White };
			float Thickness { 1.f };
		};

		using WorldLineDrawCallArray = std::array<WorldLineVertex, m_maxWorldLinesPerRender>;
		std::array<WorldLineVertex, m_maxWorldLinesPerRender> m_worldLineDrawCallList; //Just a way of having stack allocated data we can mess around with.
		WorldLineDrawCallArray::iterator m_worldLineDrawBegin;
		WorldLineDrawCallArray::iterator m_worldLineDrawItr;

		//////////////////////////////////////////////////////////////////////////
		//Circle Shader
		//////////////////////////////////////////////////////////////////////////

		SB::Shared<ShaderProgram> m_circleShader { nullptr };
		unsigned int m_circleDrawCallCount { 0 };
		std::shared_ptr<VertexBuffer> m_circleVbo { nullptr };
		std::shared_ptr<VertexArrayObject> m_circleVao { nullptr };

		static constexpr unsigned int m_maxCirclesPerRender = 1000;

		struct CircleVertex
		{
			glm::vec3 Position { 0.f };
			glm::vec4 Colour { Colours::White };
			float Radius { 1.f };
			int PointCount { 3 };
		};

		using CircleDrawCallArray = std::array<CircleVertex, m_maxCirclesPerRender>;
		std::array<CircleVertex, m_maxCirclesPerRender> m_circleDrawCallList; //Just a way of having stack allocated data we can mess around with.
		CircleDrawCallArray::iterator m_circleDrawBegin;
		CircleDrawCallArray::iterator m_circleDrawItr;

		//////////////////////////////////////////////////////////////////////////
		//Screen Point Shader
		//////////////////////////////////////////////////////////////////////////

		SB::Shared<ShaderProgram> m_screenPointShader { nullptr };
		unsigned int m_screenPointDrawCallCount { 0 };
		std::shared_ptr<VertexBuffer> m_screenPointVbo { nullptr };
		std::shared_ptr<VertexArrayObject> m_screenPointVao { nullptr };

		static constexpr unsigned int m_maxScreenPointsPerRender = 10000;

		struct ScreenPointVertex
		{
			glm::vec2 Position { 0.f };
			glm::vec4 Colour { Colours::White };
			float Size { 1.f };
		};

		using ScreenPointDrawCallArray = std::array<ScreenPointVertex, m_maxScreenPointsPerRender>;
		std::array<ScreenPointVertex, m_maxScreenPointsPerRender> m_screenPointDrawCallList; //Just a way of having stack allocated data we can mess around with.
		ScreenPointDrawCallArray::iterator m_screenPointDrawBegin;
		ScreenPointDrawCallArray::iterator m_screenPointDrawItr;

		//////////////////////////////////////////////////////////////////////////
		//World Point Shader
		//////////////////////////////////////////////////////////////////////////

		SB::Shared<ShaderProgram> m_worldPointShader { nullptr };
		unsigned int m_worldPointDrawCallCount { 0 };
		std::shared_ptr<VertexBuffer> m_worldPointVbo { nullptr };
		std::shared_ptr<VertexArrayObject> m_worldPointVao { nullptr };

		static constexpr unsigned int m_maxWorldPointsPerRender = 100000;

		struct WorldPointVertex
		{
			glm::vec3 Position { 0.f };
			glm::vec4 Colour { Colours::White };
			float Size { 1.f };
		};

		using WorldPointDrawCallArray = std::array<WorldPointVertex, m_maxWorldPointsPerRender>;
		std::array<WorldPointVertex, m_maxWorldPointsPerRender> m_worldPointDrawCallList; //Just a way of having stack allocated data we can mess around with.
		WorldPointDrawCallArray::iterator m_worldPointDrawBegin;
		WorldPointDrawCallArray::iterator m_worldPointDrawItr;

		//////////////////////////////////////////////////////////////////////////

		glm::ivec2 m_frameBufferDimensons { 800, 600 };
		glm::ivec2 m_screenDimensions { 800, 600 };
		glm::ivec2 m_screenStartingPosition { 0,0 };
		glm::ivec2 m_frameBufferStartingPosition { 0,0 };

		glm::vec4 m_clearColour { 0.2f, 0.3f, 0.3f, 1.0f };

		struct CameraData
		{
			glm::mat4 ViewProjectionMatrix { 0.f };
			glm::mat4 ViewMatrix { 0.f };
			glm::mat4 ProjectionMatrix { 0.f };
			glm::vec2 ViewPortSize { 0.f };
		};

		CameraData m_cameraData {};
		std::shared_ptr<UniformBufferObject> m_cameraUBO { nullptr };

		bool m_showTextureWindow { false };

		SB::Shared<ShaderProgram> m_screenShader { nullptr };

		std::shared_ptr<VertexArrayObject> ScreenQuadVAO { nullptr };
		std::shared_ptr<VertexBuffer> ScreenQuadVBO { nullptr };
		std::shared_ptr<Framebuffer> m_Framebuffer { nullptr };

		bool m_useFrameBufferObject { true };
		bool m_firstTimeRenderingSubWindow { m_useFrameBufferObject };

		SB::Shared<TShaderResourceHandler> m_shaderResourceHandler;
		SB::Shared<TTextureResourceHandler> m_textureResourceHandler;
		SB::Shared<TMeshResourceHandler> m_meshResourceHandler; 

		std::vector<GLuint> m_boundTextures {};
	};
}
