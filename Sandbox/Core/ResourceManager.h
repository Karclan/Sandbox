#pragma once

#include <map>
#include <string>

#include "Subsystem.h"
#include "ResourceType.h"

namespace Graphics
{
    class Renderer;
}

namespace Core
{
    
    class ResourceManager : public Subsystem, public ImGuiWindowBuilder
    {
    public:
        ResourceManager() = default;
        ~ResourceManager() = default;

        ResourceId CreateOrLoadShader();
        ResourceId CreateOrLoadTexture();

    private:
        std::map<std::string, ResourceId> m_loadedResourceMap;

    };
}