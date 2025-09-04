#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

#include <glm/glm.hpp>


class Shader
{
public:
    Shader() = default;
    ~Shader();

    // Load, compile and link from two GLSL source files. Paths are treated
    // like normal file system paths (e.g. romfs:/shaders/vertex.glsl).
    bool loadFromFiles(const std::string& vertPath, const std::string& fragPath);

    GLuint program() const { return m_program; }

    // convenience wrapper
    GLint getUniformLocation(const char* name) const { return glGetUniformLocation(m_program, name); }

    void use() const
    { 
        glUseProgram(m_program); 
    }

    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(m_program, name.c_str()), value); 
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]); 
    }


private:
    std::string readFile(const std::string& path) const;
    bool compileShader(GLenum type, const char* source, GLuint& outShader) const;

    GLuint m_program{0};
};

#endif // SHADER_H