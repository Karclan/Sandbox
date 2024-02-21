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

		bool DrawLine( glm::vec3 A, glm::vec3 b, glm::vec4 colour = glm::vec4( 1.f ), float thickness = 1.f ) noexcept;
		bool DrawCircle( glm::vec3 pos, float radius, glm::vec4 colour = glm::vec4( 1.f ), int pointCount = 3.f ) noexcept;
		bool DrawPoint( glm::vec3 pos, float size, glm::vec4 colour = Colours::White ) noexcept;

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
		//Line Shader
		//////////////////////////////////////////////////////////////////////////

		SB::Shared<ShaderProgram> m_lineShader { nullptr };
		unsigned int m_lineDrawCallCount { 0 };
		std::shared_ptr<VertexBuffer> m_lineVbo { nullptr };
		std::shared_ptr<VertexArrayObject> m_lineVao { nullptr };

		static constexpr unsigned int m_maxLinesPerRender = 2000;

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
		//Point Shader
		//////////////////////////////////////////////////////////////////////////

		SB::Shared<ShaderProgram> m_pointShader { nullptr };
		unsigned int m_pointDrawCallCount { 0 };
		std::shared_ptr<VertexBuffer> m_pointVbo { nullptr };
		std::shared_ptr<VertexArrayObject> m_pointVao { nullptr };

		static constexpr unsigned int m_maxPointsPerRender = 10000;

		struct PointVertex
		{
			glm::vec3 Position { 0.f };
			glm::vec4 Colour { Colours::White };
			float Size { 1.f };
		};

		using PointDrawCallArray = std::array<PointVertex, m_maxPointsPerRender>;
		std::array<PointVertex, m_maxPointsPerRender> m_pointDrawCallList; //Just a way of having stack allocated data we can mess around with.
		PointDrawCallArray::iterator m_pointDrawBegin;
		PointDrawCallArray::iterator m_pointDrawItr;

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
