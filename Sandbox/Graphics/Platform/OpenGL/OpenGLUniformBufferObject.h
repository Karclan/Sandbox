#pragma once

#include "../../UniformBufferObject.h"

namespace Graphics
{
	class OpenGLUniformBufferObject : public UniformBufferObject
	{
	public:
		OpenGLUniformBufferObject( uint32_t size, uint32_t binding );
		~OpenGLUniformBufferObject();

		void SetData( void const * data, uint32_t size, uint32_t offset = 0 ) override;

		void Bind() override;
		void UnBind() override;

	};
}
