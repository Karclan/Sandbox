#pragma once

#include <string>
#include <cstdint>

namespace Graphics
{
    class Shader
    {
    public:
        enum class Type : int
        {
            Vertex,
            Fragment,
            Geometry,

            Total,

            Invalid = -1
        };

        Shader();
        Shader( Type type, char const * filePath );
        ~Shader();

        unsigned int getId() const
        {
            return m_Id;
        }

        Type getType() const
        {
            return m_type;
        }

        bool create( Type type, char const * filePath );

        void cleanUp();

    private:
        void load( char const * filePath );

        unsigned int m_Id { 0 };
        Type m_type { Type::Invalid };

        std::string m_shaderSource {};
        std::string m_shaderFilePath {};
    };
}
