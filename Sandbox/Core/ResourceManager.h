#pragma once

#include <map>
#include <string>

namespace Graphics
{
    class Renderer;
}

namespace Core
{
    using ResourceId = unsigned int;

    class ResourceManager
    {
    public:
        ResourceManager();
        ~ResourceManager();

        ResourceId createOrLoadShader();
        ResourceId createOrLoadTexture();

    private:
        std::map<std::string, ResourceId> m_loadedResourceMap;

    };
}