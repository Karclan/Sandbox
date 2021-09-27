#pragma once

#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <map>
#include <bitset>


#include "../Utils/EnumBitmask.h"

namespace Graphics
{
    class ShaderProgram
    {
    public:    
        ShaderProgram();
        ShaderProgram( const char * shaderName, Utils::EnumBitmask<Shader::Type> flags );
        ~ShaderProgram();

        unsigned int getId() const;

        void useProgram() const;

        void setUniform( const char * name, float x, float y, float z );
        void setUniform( const char * name, glm::vec2 const & v );
        void setUniform( const char * name, glm::vec3 const & v );
        void setUniform( const char * name, glm::vec4 const & v );
        void setUniform( const char * name, glm::mat4 const & m );
        void setUniform( const char * name, glm::mat3 const & m );
        void setUniform( const char * name, float val );
        void setUniform( const char * name, int val );
        void setUniform( const char * name, bool val );
        void setUniform( const char * name, GLuint val );

        void printActiveUniforms() const;
        void printActiveUniformBlocks() const;
        void printActiveAttribs() const;

    private:
        bool create();
        bool attachShader( std::vector<Shader>& shaders );

        GLint  getUniformLocation( const char * name );
        const char * getTypeString( GLenum type ) const;

        unsigned int m_Id {};

        std::string m_shaderName{};
        Utils::EnumBitmask<Shader::Type> m_shaderTypeFlags;

        std::vector<Shader> m_shaders {};

        using UniformMap = std::map<std::string, int>;
        UniformMap uniformLocations {};


    };
}
