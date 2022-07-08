#ifndef SHADER_H
#define SHADER_H

// Third-party Headers
#include "thirdparty/glad/glad.h"
#include "thirdparty/glm/glm.hpp"
#include "thirdparty/glm/gtc/matrix_transform.hpp"
#include "thirdparty/glm/gtc/type_ptr.hpp"

// Custom Headers
#include "rendering/Texture.h"
#include "Config.h"
#include "utility/FileSystem.h"

// Standard Headers
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// Types of Shaders
enum SHADER_TYPE
{
    VERTEX_SHADER,
    FRAGMENT_SHADER,
    COMBINED_SHADER,
};

// Shader Class
class Shader
{
public:
    unsigned int id; // ID for the shader program

    // Default Shader constructor
    Shader();
    // String path constructor for Shader
    Shader(std::string vertexPath, std::string fragmentPath);
    // Char* path constructor for Shader
    Shader(const char *vertexPath, const char *fragmentPath);
    // Create shader program from path
    void create_shader(const char *vertexPath, const char *fragmentPath);
    // Compile individual shader code files
    unsigned int compile_shader(const char *code, SHADER_TYPE type);
    // Bind current shader ID to renderer
    void use();
    // Free shader program data
    void free_data();
    // Set a bool uniform in shader
    void set_bool(const std::string name, bool value);
    // Set a int uniform in shader
    void set_int(const std::string name, int value);
    // Set a float uniform in shader
    void set_float(const std::string name, float value);
    // Set a vec2 uniform in shader
    void set_vec2(const std::string name, float x, float y);
    // Set a vec2 uniform in shader
    void set_vec2(const std::string name, glm::vec2 value);
    // Set a vec3 uniform in shader
    void set_vec3(const std::string name, float x, float y, float z);
    // Set a vec3 uniform in shader
    void set_vec3(const std::string name, glm::vec3 value);
    // Set a vec4 uniform in shader
    void set_vec4(const std::string name, float x, float y, float z, float w);
    // Set a vec4 uniform in shader
    void set_vec4(const std::string name, glm::vec4 value);
    // Set a mat2 uniform in shader
    void set_mat2(const std::string name, glm::mat2 value);
    // Set a mat3 uniform in shader
    void set_mat3(const std::string name, glm::mat3 value);
    // Set a mat4 uniform in shader
    void set_mat4(const std::string name, glm::mat4 value);
    // Set a texture uniform in shader
    void set_texture(const std::string name, Texture *tex);
    // Sets the matrices for a 3D object
    void set_matrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    // Sets the material for a 3D object
    void set_material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

private:
    // Check for compilation errors in shader
    bool check_compile_errors(unsigned int shader, SHADER_TYPE type);
};

// Struct for a Field in Material class
struct MaterialField
{
    glm::vec3 color; // Color for the field
    Texture tex;     // Texture for the field

    // Default MaterialField Constructor
    MaterialField(glm::vec3 color_ = DEFAULT_SHADER_COLOR) : color(color_) {}
};

// Templates of shader
enum SHADER_TEMPLATE
{
    COLOR_SHADER_3D,
    TEXTURE_SHADER_3D,
};

// Array of file name for the vertex shaders
extern std::string vShaderNames[LOADED_SHADERS_COUNT];

// Array of file name for the fragment shaders
extern std::string fShaderNames[LOADED_SHADERS_COUNT];

// Shader names to use for UI
static const char *shaderNames[] = {"Color Shader", "Texture Shader"};

// Material Class for Actor
class Material
{
public:
    MaterialField ambient;  // Ambient color for Mat
    MaterialField diffuse;  // Diffuse color for Mat
    MaterialField specular; // Specular color for Mat
    float shininess;        // Shininess factor for Mat
    SHADER_TEMPLATE shader; // Type of shader used by this material

    // Default Material Constructor
    Material();
    // Color Material Constructor
    Material(glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, float shininess_ = 64.0f);
};

// LightSource class for Shader
class LightSource
{
public:
    glm::vec3 position; // Position of light source
    glm::vec3 ambient;  // Ambient light col for source
    glm::vec3 diffuse;  // Diffuse light col for source
    glm::vec3 specular; // Specular light col for source

    // Default LightSource Constructor
    LightSource();
    // Color LightSource Constructor
    LightSource(glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_, glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f));
};

#endif // !SHADER_H
