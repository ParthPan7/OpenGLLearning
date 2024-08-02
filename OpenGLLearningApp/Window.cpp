#include "Window.h"



Window::Window()
{
    height = 1600;
    width = 1800;
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
    xChange = 0.0f;
    yChange = 0.0f;
    hasMouseMovedFirstTime = false;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
}

int Window::IntializeGLFWWindow()
{
    //initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW Initialization failed!");
        glfwTerminate();
        return 1;
    }

    // Set up GLFW Windows properties 
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Core profile = no backward compatible 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    //Get Buffer Size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);
    SetEventCallBacks();
    //Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation has failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    //Set Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    glfwSetWindowUserPointer(mainWindow, this);
}

int Window::GetBufferWidth()
{
    return bufferWidth;
}

int Window::GetBufferHeight()
{
    return bufferHeight;
}

bool Window::ShouldWindowClose()
{
    return glfwWindowShouldClose(mainWindow);
}

void Window::WindowSwapBuffers()
{
   glfwSwapBuffers(mainWindow);
}

void Window::handleKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (theWindow->hasMouseMovedFirstTime == true)
    {
        theWindow->xLast = xPos;
        theWindow->yLast = yPos;
        theWindow->hasMouseMovedFirstTime = false;
    }
    theWindow->xChange = xPos - theWindow->xLast;
    theWindow->yChange = theWindow->yLast - yPos;

    theWindow->xLast = xPos;
    theWindow->yLast = yPos;
}

GLfloat Window::GetXChange()
{
    float theXChange = xChange;
    xChange = 0.0f;
    return theXChange;
}

GLfloat Window::GetYChange()
{
    float theYChange = yChange;
    yChange = 0.0f;
    return theYChange;
}

bool* Window::GetKeys()
{
    return keys;
}

void Window::SetEventCallBacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}