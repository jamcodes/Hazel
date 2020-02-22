#include "OpenGLShader.h"

#include "Hazel/Log.h"
#include "Hazel/AssertionHandler.h"

#include <string_view>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace {
struct ShaderAssertHandler final : Hazel::CoreLoggingHandler, Hazel::Enforce {
};
}  // namespace

namespace Hazel {
OpenGLShader::OpenGLShader(const std::string& vertex_src, const std::string& fragment_src)
// TODO: Make this noexcept conditionally -> if ShaderAssertHandler::handle is nothrow
{
    // Create an empty vertex shader handle
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar *source = static_cast<const GLchar *>(vertex_src.c_str());
    glShaderSource(vertex_shader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertex_shader);

    GLint is_compiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        GLint max_length = 0;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

        // The max_length includes the NULL character
        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(vertex_shader, max_length, &max_length, &info_log[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertex_shader);

        HZ_CORE_ERROR("{}", info_log.data());
        HZ_ASSERT(false, ShaderAssertHandler, Hazel::Enforce, "Vertex shader compilation failed");
        // TODO: throw here instead? Or create a throwing assert-handler
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = static_cast<const GLchar *>(fragment_src.c_str());
    glShaderSource(fragment_shader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        GLint max_length = 0;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

        // The max_length includes the NULL character
        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(fragment_shader, max_length, &max_length, &info_log[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragment_shader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertex_shader);

        HZ_CORE_ERROR("{}", info_log.data());
        HZ_ASSERT(false, ShaderAssertHandler, Hazel::Enforce, "Fragment shader compilation failed");
        // TODO: throw here instead? Or create a throwing assert-handler
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    renderer_id_ = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(renderer_id_, vertex_shader);
    glAttachShader(renderer_id_, fragment_shader);

    // Link our program
    glLinkProgram(renderer_id_);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint is_linked{0};
    glGetProgramiv(renderer_id_, GL_LINK_STATUS, static_cast<int *>(&is_linked));
    if (is_linked == GL_FALSE) {
        GLint max_length{0};
        glGetProgramiv(renderer_id_, GL_INFO_LOG_LENGTH, &max_length);

        // The max_length includes the NULL character
        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(renderer_id_, max_length, &max_length, &info_log[0]);

        // We don't need the program anymore.
        glDeleteProgram(renderer_id_);
        // Don't leak shaders either.
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        // Use the info_log as you see fit.

        // In this simple program, we'll just leave
        HZ_CORE_ERROR("{}", info_log.data());
        HZ_ASSERT(false, ShaderAssertHandler, Hazel::Enforce, "OpenGLShader link failed");
        // TODO: throw here instead? Or create a throwing assert-handler
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(renderer_id_, vertex_shader);
    glDetachShader(renderer_id_, fragment_shader);
}

OpenGLShader::~OpenGLShader() noexcept { glDeleteProgram(renderer_id_); }

void OpenGLShader::bind() const { glUseProgram(renderer_id_); }

void OpenGLShader::unbind() const { glUseProgram(0); }

void OpenGLShader::uploadUniform(std::string const& name, int value) const
{
    GLint location{glGetUniformLocation(renderer_id_, name.c_str())};
    glUniform1i(location, value);
}

void OpenGLShader::uploadUniform(std::string const& name, float value) const
{
    GLint location{glGetUniformLocation(renderer_id_, name.c_str())};
    glUniform1f(location, value);
}
void OpenGLShader::uploadUniform(std::string const& name, glm::vec2 const& values) const
{
    GLint location{glGetUniformLocation(renderer_id_, name.c_str())};
    glUniform2f(location, values.x, values.y);
}
void OpenGLShader::uploadUniform(std::string const& name, glm::vec3 const& values) const
{
    GLint location{glGetUniformLocation(renderer_id_, name.c_str())};
    glUniform3f(location, values.x, values.y, values.z);
}
void OpenGLShader::uploadUniform(std::string const& name, glm::vec4 const& values) const
{
    GLint location{glGetUniformLocation(renderer_id_, name.c_str())};
    glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::uploadUniform(std::string const& name, glm::mat3 const& matrix) const
{
    GLint location{glGetUniformLocation(renderer_id_, name.c_str())};
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::uploadUniform(std::string const& name, glm::mat4 const& matrix) const
{
    auto location{glGetUniformLocation(renderer_id_, name.c_str())};
    HZ_ASSERT(-1 != location, DefaultCoreHandler, Enforce, "Unknown uniform name");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}



}  // namespace Hazel
