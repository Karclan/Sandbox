#include "MeshResourceHandler.h"

#include "Triangle.h"
#include "Square.h"
#include "Cube.h"

namespace Graphics
{

	TMeshResourceHandler::TMeshResourceHandler()
	{
		m_triangleMesh = SB::CreateShared<Triangle>();
		m_squareMesh = SB::CreateShared<Square>();
		m_cubeMesh = SB::CreateShared<Cube>();
	}

	SB::Shared<Graphics::Mesh> TMeshResourceHandler::GetOrLoadResource( char const * resourceName )
	{
		SB::Shared<Graphics::Mesh> MeshPtr;

		if ( magic_enum::enum_contains<PrimativeType>( resourceName ) )
		{
			PrimativeType type = magic_enum::enum_cast<PrimativeType>( resourceName ).value_or(PrimativeType::Invalid);
			MeshPtr = GetMesh( type );
		}
		else
		{

		}

		return MeshPtr;
	}

	SB::Shared<Graphics::Mesh> TMeshResourceHandler::GetMesh( const char * meshPath )
	{
		SB::Shared<Graphics::Mesh> MeshPtr;
		LoadMesh( meshPath, MeshPtr );
		return MeshPtr;
	}

	SB::Shared<Graphics::Mesh> TMeshResourceHandler::GetMesh( PrimativeType Type )
	{
		switch ( Type )
		{
			case Graphics::PrimativeType::Square:
				return m_triangleMesh;
				break;
			case Graphics::PrimativeType::Triangle:
				return m_squareMesh;
				break;
			case Graphics::PrimativeType::Cube:
				return m_cubeMesh;
				break;
			case Graphics::PrimativeType::Invalid:
			default:
				SB_CORE_ASSERT( false, "Unhandled PrimativeType returned, defaulting to Triangle mesh" );
				return m_triangleMesh;
				break;
		}
	}

	void TMeshResourceHandler::BuildWindow()
	{

	}

	Graphics::TMeshResourceHandler::MeshMap const & TMeshResourceHandler::GetLoadedMeshes()
	{
		return m_loadedMeshes;
	}

	bool TMeshResourceHandler::LoadMesh( const std::string & meshPath, SB::Shared<Mesh> & outMesh )
	{
		//#TODO_Mesh: Mesh loading
		return false;
	}

	bool TMeshResourceHandler::FindMesh( const std::string & meshPath, SB::Shared<Mesh> & outMesh )
	{
		MeshMap::iterator findResult = m_loadedMeshes.find( meshPath );
		if ( findResult != m_loadedMeshes.end() )
		{
			outMesh = findResult->second;
			return true;
		}

		return false;
	}

}
