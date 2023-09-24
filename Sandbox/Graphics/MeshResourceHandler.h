#pragma once

#include "../Core/ResourceManager.h"

#include "Mesh.h"
#include "MeshComponent.h"

namespace Graphics
{
	class TMeshResourceHandler : public Core::TResourceTypeHandler<Core::ResourceType::Mesh, SB::Shared<Mesh>>
	{
	public:
		TMeshResourceHandler();
		~TMeshResourceHandler() = default;

		SB::Shared<Mesh> GetOrLoadResource( char const * resourceName ) override;
		SB::Shared<Mesh> GetMesh( const char * meshPath );

		//Returns one of the hard coded Primitive meshes
		SB::Shared<Mesh> GetMesh( PrimativeType Type );

		void BuildWindow() override;

		using MeshMap = std::unordered_map<std::string, SB::Shared<Mesh>>;
		MeshMap const & GetLoadedMeshes();
	private:
		bool LoadMesh( const std::string & meshPath, SB::Shared<Mesh> & outMesh );
		bool FindMesh( const std::string & meshPath, SB::Shared<Mesh> & outMesh );

		SB::Shared<Mesh> m_triangleMesh;
		SB::Shared<Mesh> m_squareMesh;
		SB::Shared<Mesh> m_cubeMesh;

		MeshMap m_loadedMeshes;

	};
}