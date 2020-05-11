#include "OpenGLShader.h"

#include <glad/glad.h>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <string_view>
#include <vector>

#include "Hazel/Core/AssertionHandler.h"
#include "Hazel/Core/Log.h"

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

    // get name from filepath
    auto last_slash{filepath.find_last_of("/\\")};
    last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
    auto const last_dot{filepath.rfind('.')};
    auto count{last_dot == std::string::npos ? filepath.size() - last_slash
                                             : last_dot - last_slash};
    name_ = filepath.substr(last_slash, count);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex_src,
                           const std::string& fragment_src)
    // TODO: Make this noexcept conditionally -> if ShaderAssertHandler::handle is nothrow
    : name_{name}
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
    auto pos{shader_src.find(type_token, 0)};  // Start of shader type declaration line
    while (pos != std::string::npos) {
        // End of shader type declaration line
        const auto eol{shader_src.find_first_of("\r\n", pos)};
        HZ_ASSERT(eol != std::string::npos, ShaderAssertHandler, Hazel::Enforce, "Syntax error");
        // Start of shader type name (after "#type " keyword)
        const auto begin{pos + type_token_length + 1};
        const auto type{shader_src.substr(begin, eol - begin)};
        HZ_ASSERT(shaderTypeFromString(type), ShaderAssertHandler, Hazel::Enforce,
                  "Invalid shader type");

        // Start of shader code after shader type declaration line
        auto const next_line_pos = shader_src.find_first_not_of("\r\n", eol);
        HZ_ASSERT(next_line_pos != std::string::npos, ShaderAssertHandler, Hazel::Enforce,
                  "Syntax error");
        // Start of next shader type declaration line
        pos = shader_src.find(type_token, next_line_pos);

        auto const insert_res = shader_sources.insert(
            {shaderTypeFromString(type),
             (pos == std::string::npos) ? shader_src.substr(next_line_pos)
                                        : shader_src.substr(next_line_pos, pos - next_line_pos)});
        HZ_ASSERT(insert_res.second, ShaderAssertHandler, Hazel::Enforce,
                  "Multiple blocks of the same type in a single shader file");
    }
    return shader_sources;
}

void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shader_src)
{
    std::array<GLenum, 4> gl_shader_ids;
    HZ_ASSERT(shader_src.size() <= gl_shader_ids.size(), ShaderAssertHandler, Hazel::Enforce,
              "Shader ID buffer overflow. Allocate a larger buffer");
    // std::vector<GLenum> gl_shader_ids{};
    // gl_shader_ids.reserve(shader_src.size());
    auto index{0};
    for (const auto& [type, src] : shader_src) {
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
        gl_shader_ids[index++] = std::move(shader);
    }

    // maybe this?
    // const auto foreach_shader =
    //     [&shaders = std::as_const(gl_shader_ids), idx=index](auto&& func) noexcept {
    //         for (auto i{0}; i < idx; ++i) {
    //             func(shaders[i]);
    //         }
    //     };
    // foreach_shader([p=program](auto const id) noexcept {glAttachShader(p, id);});

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    const GLuint program{glCreateProgram()};
    // Attach our shaders to our program
    for (auto i{0}; i < index; ++i) {
        glAttachShader(program, gl_shader_ids[i]);
    }
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
        for (auto i{0}; i < index; ++i) {
            glDeleteShader(gl_shader_ids[i]);
        }
        // Use the info_log as you see fit.

        // In this simple program, we'll just leave
        HZ_CORE_ERROR("{}", info_log.data());
        HZ_ASSERT(false, ShaderAssertHandler, Hazel::Enforce, "OpenGLShader link failed");
        // TODO: throw here instead? Or create a throwing assert-handler
        return;
    }

    // Always detach shaders after a successful link.
    while (index) {
        glDetachShader(program, gl_shader_ids[--index]);
        glDeleteShader(gl_shader_ids[index]);
    }
    renderer_id_ = program;
}

void OpenGLShader::bind() const { glUseProgram(renderer_id_); }

void OpenGLShader::unbind() const { glUseProgram(0); }

void OpenGLShader::setUniform(std::string const& name, int value)
{
    uploadUniform(name, value);
}

void OpenGLShader::setUniform(std::string const& name, float value)
{
    uploadUniform(name, value);
}

void OpenGLShader::setUniform(std::string const& name, glm::vec2 const& values)
{
    uploadUniform(name, values);
}

void OpenGLShader::setUniform(std::string const& name, glm::vec3 const& values)
{
    uploadUniform(name, values);
}

void OpenGLShader::setUniform(std::string const& name, glm::vec4 const& values)
{
    uploadUniform(name, values);
}

void OpenGLShader::setUniform(std::string const& name, glm::mat3 const& uniform)
{
    uploadUniform(name, uniform);
}

void OpenGLShader::setUniform(std::string const& name, glm::mat4 const& uniform)
{
    uploadUniform(name, uniform);
}


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
