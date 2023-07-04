#pragma once

#include "MeshComponent.h"

#include "VertexArrayObject.h"
#include "DataBuffer.h"

namespace Graphics
{
	class Mesh
	{
	public:
		Mesh() = default;
		virtual ~Mesh();

		std::shared_ptr<VertexArrayObject> const & GetVAO() { return m_VAO; }

		virtual void Create() = 0;
		virtual void Bind();
		void CleanUp();

	protected:
		void FinishBinding();

		void SetCreated( bool flag );
		bool GetCreated() const;

		bool m_Created { false };

		std::shared_ptr<VertexArrayObject> m_VAO { nullptr };
		//Should remove these from the Mesh and put them on the sub objects really. 
		std::shared_ptr<VertexBuffer> m_VBO { nullptr };
		std::shared_ptr<IndexBuffer> m_EBO { nullptr };
	};
}
