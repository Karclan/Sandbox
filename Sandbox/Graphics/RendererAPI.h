#pragma once

namespace Graphics
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL,
			DirectX,
			Vulkan,
		};

		static API GetAPI() { return s_API; }

	private:
		
		static API s_API;
	};
}