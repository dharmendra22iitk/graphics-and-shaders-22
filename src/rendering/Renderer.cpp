#include "rendering/Renderer.h"

static RenderCamera rCam;

Renderer::Renderer(int major_, int minor_, int width_, int height_)
{
    major = major_;
    minor = minor_;
    width = width_;
    height = height_;
}

void Renderer::initialise_glfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void Renderer::terminate_glfw()
{
    glfwTerminate();
}

bool Renderer::create_window()
{
#if ENABLE_FULLSCREEN
    window = glfwCreateWindow(width, height, WINDOW_NAME, glfwGetPrimaryMonitor(), NULL);
#else
    window = glfwCreateWindow(width, height, WINDOW_NAME, NULL, NULL);
#endif

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        terminate_glfw();
        return false;
    }
    return true;
}

void Renderer::setup_window_data()
{
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
#if ((!ENABLE_FULLSCREEN) * ENABLE_FIXED_ASPECT_RATIO)
    glfwSetWindowAspectRatio(window, ASPECT_RATIO_X, ASPECT_RATIO_Y);
#endif
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
}

bool Renderer::close_window()
{
    return glfwWindowShouldClose(window);
}

void Renderer::swap_buffers(bool lockFrameRate)
{
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (!lockFrameRate)
    {
        glfwSwapInterval(0);
    }
    else
    {
        glfwSwapInterval(1);
    }
}

bool Renderer::check_key(int key)
{
    return (glfwGetKey(window, key) == GLFW_PRESS);
}

void Renderer::start_timer()
{
    currentTime = glfwGetTime();
    previousTime = currentTime;
    deltaTime = 0.0f;
}

void Renderer::new_frame()
{
    currentTime = glfwGetTime();
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;
}

void Renderer::set_camera(Camera cam)
{
    rCam.cam = Camera(cam.position);
    rCam.lastX = width / 2.0f;
    rCam.lastY = height / 2.0f;
    rCam.isFirstMouse = true;
    rCam.xOffset = 0.0f;
    rCam.yOffset = 0.0f;
}

Camera *Renderer::get_camera()
{
    return &(rCam.cam);
}

void Renderer::set_cursor(bool status)
{
    if (status)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void Renderer::process_mouse(bool isActive)
{
    if (isActive)
    {
        rCam.cam.process_mouse(rCam.xOffset, rCam.yOffset, deltaTime);
        set_cursor(false);
    }
    else
    {
        set_cursor(true);
    }

    rCam.xOffset = 0.0f;
    rCam.yOffset = 0.0f;
}

//------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    rCam.xOffset = xpos - rCam.lastX;
    rCam.yOffset = ypos - rCam.lastY;
    rCam.lastX = xpos;
    rCam.lastY = ypos;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    rCam.cam.process_scroll(yoffset);
}

//---------------------------------------------------------

void VertexArray::generate_buffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void VertexArray::bind_vao()
{
    glBindVertexArray(VAO);
}

void VertexArray::unbind_vao()
{
    glBindVertexArray(0);
}

void VertexArray::bind_vbo(int vertexCount, GLsizeiptr stride, void *pointer)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * stride, pointer, GL_STATIC_DRAW);
}

void VertexArray::unbind_vbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::bind_ebo(int indexCount, void *pointer)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), pointer, GL_STATIC_DRAW);
}

void VertexArray::unbind_ebo()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexArray::set_attribute_array(int layoutLayer, int count, GLsizeiptr stride, const void *pointer)
{
    glEnableVertexAttribArray(layoutLayer);
    glVertexAttribPointer(layoutLayer, count, GL_FLOAT, GL_FALSE, stride, pointer);
}

void VertexArray::draw_triangle(int count, int startIndex)
{
    bind_vao();
    glDrawArrays(GL_TRIANGLES, startIndex, count);
    unbind_vao();
}

void VertexArray::draw_indices(int indexCount)
{
    bind_vao();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    unbind_vao();
}

void VertexArray::free_data()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
