    #include <GLFW/glfw3.h>

// Function prototype for the window size callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Set the window size callback */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Calculate the desired aspect ratio
    float aspectRatio = 4.0f / 3.0f;

    // Calculate the new width and height based on the aspect ratio
    int newWidth = width;
    int newHeight = static_cast<int>(width / aspectRatio);

    // Adjust the viewport to maintain the aspect ratio
    int xOffset = 0;
    int yOffset = (height - newHeight) / 2;
    glViewport(xOffset, yOffset, newWidth, newHeight);
}
