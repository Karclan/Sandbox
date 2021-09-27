#include "Renderable.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <algorithm>

namespace Graphics
{
    /*int Renderable::count = 0;
    Renderable::Renderable( Mesh const * mesh, TransformComponent const * transform )
        : m_mesh( mesh )
        , m_transform( transform )
        , m_shaderId( nullptr )
        , m_textureList()
    {
        m_renderableId = ++count;
    }

    Renderable::~Renderable()
    {

    }

    void Renderable::ImGuiRender() const
    {
        std::string name = "Renderable_";
        
        name.append( std::to_string( m_renderableId ) );
        ImGui::Begin("Renderables");
        ImGui::BeginGroup( );
        ImGui::Text(name.c_str());
        ImGui::EndGroup();
        ImGui::End();
    }

    void Renderable::assignTexture( std::initializer_list<unsigned int> textures )
    {
        m_textureList = textures;
        std::sort( m_textureList.begin(), m_textureList.end() );
    }

    void Renderable::assignShader( ShaderProgram * shaderId )
    {
        m_shaderId = shaderId;
    }

    std::vector<unsigned int> const & Renderable::getTextures() const
    {
        return m_textureList;
    }

    ShaderProgram * Renderable::getShader() const
    {
        return m_shaderId;
    }

    Mesh const * Renderable::getMesh() const
    {
        return m_mesh;
    }

    TransformComponent const * Renderable::getTransform() const
    {
        return m_transform;
    }*/

}
