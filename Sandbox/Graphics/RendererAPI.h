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

		static constexpr API GetAPI() { return s_API; }
	private:
		static constexpr API s_API = API::OpenGL;
	};
}
