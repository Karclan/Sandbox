#pragma once

#include "glm/ext/vector_int2.hpp"

namespace Graphics
{
	//A single creation render stats component
	//Containing info such as viewport size
	//Provides a way to query info about the render
	//Without having to have direct dependencies on the rendering system itself
	struct RenderStats
	{
		glm::ivec2 ScreenDimensions { 0 };
		glm::ivec2 FrameBufferDimensions { 0 };

		glm::ivec2 WindowPosition { 0 };
		glm::ivec2 FramebufferStartingPosition { 0 };

		glm::ivec2 ViewportDimensions { 0 }; //The current dimensions we're using whether that be screen size or Framebuffer size (sub window rendering in editor)
		glm::ivec2 ViewportStartingPosition { 0 }; //The current starting coordinate we're using whether that be screen pos or sub window position "Framebuffer rendered window" (sub window rendering in editor)
		bool m_renderingSubwindow { false };
	};
}