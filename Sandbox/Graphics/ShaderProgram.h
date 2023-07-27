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
    using ShaderId = unsigned int;

	class IShaderHandle
	{
    public:
        virtual ShaderId GetShaderId() const = 0;

        virtual std::string const & GetShaderName() const = 0;

        virtual std::string const & GetShaderPath( Shader::Type type ) const = 0;
        virtual std::string const & GetShaderSource( Shader::Type type ) const = 0;

        virtual Utils::EnumBitmask<Shader::Type> const & GetShaderTypes() const = 0;
        virtual std::map<std::string, int> const & GetUniforms() const = 0;

		virtual std::map<std::string, bool> const & GetBoolUniforms() const = 0;
		virtual std::map<std::string, int> const & GetIntUniforms() const = 0;
		virtual std::map<std::string, float> const & GetFloatUniforms() const = 0;
		virtual std::map<std::string, GLuint> const & GetGLuintUniforms() const = 0;
		virtual std::map<std::string, glm::vec2> const & GetVec2Uniforms() const = 0;
		virtual std::map<std::string, glm::vec3> const & GetVec3Uniforms() const = 0;
		virtual std::map<std::string, glm::vec4> const & GetVec4Uniforms() const = 0;
		virtual std::map<std::string, glm::mat3> const & GetMat3Uniforms() const = 0;
		virtual std::map<std::string, glm::mat4> const & GetMat4Uniforms() const = 0;
        
	};

    class ShaderProgram : public IShaderHandle
    {
    public:    

        ShaderProgram( std::string const & shaderName, Utils::EnumBitmask<Shader::Type> flags );
        ~ShaderProgram() = default;

        std::vector<Shader> const & GetPrograms() const { return m_shaders; }

        void useProgram() const noexcept;

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

        //Inherited Functions from IShaderHandle
        //---------------------------------------------------------------------//

		ShaderId GetShaderId() const override { return m_Id; }

        std::string const & GetShaderName() const override; 

        std::string const & GetShaderPath( Shader::Type type ) const override;
        std::string const & GetShaderSource( Shader::Type type ) const override;

        Utils::EnumBitmask<Shader::Type> const & GetShaderTypes() const override;
		std::map<std::string, int> const & GetUniforms() const override;
		std::map<std::string, bool> const & GetBoolUniforms() const override;
		std::map<std::string, int> const & GetIntUniforms() const override;
		std::map<std::string, float> const & GetFloatUniforms() const override;
		std::map<std::string, GLuint> const & GetGLuintUniforms() const override;
		std::map<std::string, glm::vec2> const & GetVec2Uniforms() const override;
		std::map<std::string, glm::vec3> const & GetVec3Uniforms() const override;
		std::map<std::string, glm::vec4> const & GetVec4Uniforms() const override;
		std::map<std::string, glm::mat3> const & GetMat3Uniforms() const override;
		std::map<std::string, glm::mat4> const & GetMat4Uniforms() const override;
        //---------------------------------------------------------------------//
    private:
        bool create();
        bool attachShader( std::vector<Shader> const & shaders );

        GLint  getUniformLocation( const char * name );
        const char * getTypeString( GLenum type ) const noexcept;

        ShaderId m_Id {};

        std::string m_shaderName{};
        Utils::EnumBitmask<Shader::Type> m_shaderTypeFlags;

		std::vector<Shader> m_shaders {};

		using UniformMap = std::map<std::string, int>;
		UniformMap uniformLocations {};

		std::map<std::string, bool> m_boolUniforms = {};
		std::map<std::string, int> m_intUniforms = {};
		std::map<std::string, float> m_floatUniforms = {};
		std::map<std::string, GLuint> m_gluintUniforms = {};
		std::map<std::string, glm::vec2> m_vec2Uniforms = {};
		std::map<std::string, glm::vec3> m_vec3Uniforms = {};
		std::map<std::string, glm::vec4> m_vec4Uniforms = {};
		std::map<std::string, glm::mat3> m_mat3Uniforms = {};
		std::map<std::string, glm::mat4> m_mat4Uniforms = {};
    };
}
