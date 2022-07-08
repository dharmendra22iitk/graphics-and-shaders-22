#include "rendering/Shader.h"

Shader::Shader()
{
}

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
    std::cout << vertexPath << fragmentPath;
    create_shader(vertexPath.c_str(), fragmentPath.c_str());
}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    create_shader(vertexPath, fragmentPath);
}

void Shader::create_shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);

        std::stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Error Shader File Not loaded successfully" << std::endl;
    }

    unsigned int vertex, fragment;

    vertex = compile_shader(vertexCode.c_str(), VERTEX_SHADER);
    if (check_compile_errors(vertex, VERTEX_SHADER))
    {
        return;
    }

    fragment = compile_shader(fragmentCode.c_str(), FRAGMENT_SHADER);
    if (check_compile_errors(fragment, FRAGMENT_SHADER))
    {
        return;
    }

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);

    glLinkProgram(id);
    if (check_compile_errors(id, COMBINED_SHADER))
    {
        return;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::compile_shader(const char *code, SHADER_TYPE type)
{
    unsigned int shader;

    if (type == VERTEX_SHADER)
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == FRAGMENT_SHADER)
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }

    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    return shader;
}

void Shader::use()
{
    glUseProgram(id);
}

void Shader::free_data()
{
    glDeleteProgram(id);
}

void Shader::set_bool(const std::string name, bool value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), int(value));
}

void Shader::set_int(const std::string name, int value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), int(value));
}

void Shader::set_float(const std::string name, float value)
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set_vec2(const std::string name, float x, float y)
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::set_vec2(const std::string name, glm::vec2 value)
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
}

void Shader::set_vec3(const std::string name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::set_vec3(const std::string name, glm::vec3 value)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

void Shader::set_vec4(const std::string name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::set_vec4(const std::string name, glm::vec4 value)
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::set_mat2(const std::string name, glm::mat2 value)
{
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::set_mat3(const std::string name, glm::mat3 value)
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::set_mat4(const std::string name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::set_texture(const std::string name, Texture *tex)
{
    set_active_texture(tex->id);
    set_int(name, tex->id);
    tex->bind_texture();
}

void Shader::set_matrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    set_mat4("model", model);
    set_mat4("view", view);
    set_mat4("projection", projection);
}

void Shader::set_material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
    set_vec3("mat.ambient", ambient);
    set_vec3("mat.diffuse", diffuse);
    set_vec3("mat.specular", specular);
    set_float("mat.shininess", shininess);
}

bool Shader::check_compile_errors(unsigned int shader, SHADER_TYPE type)
{
    int success;

    char infoLog[1024];

    if (type == COMBINED_SHADER)
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING FAILED OF TYPE" << std::endl
                      << infoLog << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILE::LINKING FAILED OF TYPE" << std::endl
                      << infoLog << std::endl;
        }
    }
    return success == 0;
}

std::string vShaderNames[] = {"shaders/3dshaders/lighting.vs",
                              "shaders/3dshaders/lighting.vs"};

std::string fShaderNames[] = {"shaders/3dshaders/lighting.fs",
                              "shaders/3dshaders/lighting_texture.fs"};

void load_template_shaders()
{
    for (int i = 0; i < LOADED_SHADERS_COUNT; i++)
    {
        Shader shdr(FileSystem::get_path(vShaderNames[i]), FileSystem::get_path(fShaderNames[i]));
        templateShaders.push_back(shdr);
    }
}

Material::Material()
{
    shininess = 64.0f;
}

Material::Material(glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, float shininess_)
{
    ambient = MaterialField(ambient_);
    diffuse = MaterialField(diffuse_);
    specular = MaterialField(specular_);
    shininess = shininess_;
}

LightSource::LightSource()
{
    ambient = DEFAULT_LIGHT_COLOR;
    diffuse = DEFAULT_LIGHT_COLOR;
    specular = DEFAULT_LIGHT_COLOR;
    position = WORLD_ORIGIN;
}

LightSource::LightSource(glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, glm::vec3 position_)
{
    ambient = ambient_;
    diffuse = diffuse_;
    specular = specular_;
    position = position_;
}
