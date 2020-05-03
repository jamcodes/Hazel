#include "OpenGLShader.h"

#include <glad/glad.h>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <string_view>
#include <vector>

#include "Hazel/AssertionHandler.h"
#include "Hazel/Log.h"

namespace {
struct ShaderAssertHandler final : Hazel::CoreLoggingHandler, Hazel::Enforce {
};
}  // namespace

namespace Hazel {
static GLenum shaderTypeFromString(const std::string& type) noexcept
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;
    HZ_ASSERT(false, ShaderAssertHandler, Hazel::Enforce, "Unknown shader type");
    return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath)
{
    const std::string source = readFile(filepath);
    const auto shader_sources{preProcess(source)};
    compile(shader_sources);
}

OpenGLShader::OpenGLShader(const std::string& vertex_src, const std::string& fragment_src)
// TODO: Make this noexcept conditionally -> if ShaderAssertHandler::handle is nothrow
{
    std::unordered_map<GLenum, std::string> sources;
    sources.insert({GL_VERTEX_SHADER, vertex_src});
    sources.insert({GL_FRAGMENT_SHADER, fragment_src});
    compile(sources);
}

OpenGLShader::~OpenGLShader() noexcept { glDeleteProgram(renderer_id_); }

std::string OpenGLShader::readFile(const std::string& filepath)
{
    std::string result;
    std::ifstream in{filepath, std::ios_base::binary};
    if (in) {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(result.data(), result.size());
        // more concise but likely slower?
        // return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    }
    else {
        HZ_CORE_ERROR("Failed to open file '{0}'", filepath);
    }
    in.close();
    return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string shader_src)
{
    std::unordered_map<GLenum, std::string> shader_sources;

    constexpr const char* type_token{"#type"};
    const auto type_token_length{std::strlen(type_token)};
    auto pos{shader_src.find(type_token)};
    while (pos != std::string::npos) {
        const auto eol{shader_src.find_first_of("\r\n", pos)};
        HZ_ASSERT(eol != std::string::npos, ShaderAssertHandler, Hazel::Enforce, "Syntax error");
        const auto begin{pos + type_token_length + 1};
        const auto type{shader_src.substr(begin, eol - begin)};
        HZ_ASSERT(shaderTypeFromString(type), ShaderAssertHandler, Hazel::Enforce,
                  "Invalid shader type");

        auto const next_line_pos = shader_src.find_first_not_of("\r\n", eol);
        pos = shader_src.find(type_token, next_line_pos);
        auto const insert_res = shader_sources.insert(
            {shaderTypeFromString(type),
             shader_src.substr(next_line_pos,
                               pos - (next_line_pos == std::string::npos ? shader_src.size() - 1
                                                                         : next_line_pos))});
        HZ_ASSERT(insert_res.second, ShaderAssertHandler, Hazel::Enforce,
                  "Multiple blocks of the same type in a single shader file");
    }
    return shader_sources;
}

void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shader_src)
{
    std::vector<GLenum> gl_shader_ids(shader_src.size());
    // GLuint program{glCreateProgram()};
    for (const auto& [type, src] : shader_src) {
        // GLenum type{kv.first};
        // const std::string& src{kv.second};

        GLuint shader = glCreateShader(type);

        const GLchar* gl_src = static_cast<const GLchar*>(src.c_str());
        glShaderSource(shader, 1, &gl_src, 0);

        // Compile the vertex shader
        glCompileShader(shader);

        GLint is_compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_FALSE) {
            GLint max_length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

            // The max_length includes the NULL character
            std::vector<GLchar> info_log(max_length);
            glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);

            // We don't need the shader anymore.
            glDeleteShader(shader);

            HZ_CORE_ERROR("{}", info_log.data());
            HZ_ASSERT(false, ShaderAssertHandler, Hazel::Enforce, "Shader compilation failed");
            // TODO: throw here instead? Or create a throwing assert-handler
            break;
        }
        // attach the shader to our program
        // glAttachShader(program, shader);
        gl_shader_ids.push_back(std::move(shader));
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    const GLuint program{glCreateProgram()};
    // renderer_id_ = program;

    // Attach our shaders to our program
    for (auto id : gl_shader_ids) {
        glAttachShader(program, id);
    }
    // glAttachShader(program, vertex_shader);
    // glAttachShader(program, fragment_shader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint is_linked{0};
    glGetProgramiv(program, GL_LINK_STATUS, static_cast<int*>(&is_linked));
    if (is_linked == GL_FALSE) {
        GLint max_length{0};
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

        // The max_length includes the NULL character
        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        for (auto id : gl_shader_ids) {
            glDeleteShader(id);
        }
        // glDeleteShader(vertex_shader);
        // glDeleteShader(fragment_shader);

        // Use the info_log as you see fit.

        // In this simple program, we'll just leave
        HZ_CORE_ERROR("{}", info_log.data());
        HZ_ASSERT(false, ShaderAssertHandler, Hazel::Enforce, "OpenGLShader link failed");
        // TODO: throw here instead? Or create a throwing assert-handler
        return;
    }

    // Always detach shaders after a successful link.
    for (auto id : gl_shader_ids) {
            glDetachShader(program ,id);
    }
    // glDetachShader(program, vertex_shader);
    // glDetachShader(program, fragment_shader);
    renderer_id_ = program;
}

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
