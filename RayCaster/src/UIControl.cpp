#include "UIControl.h"
#include "Scene.h"

#include <iostream>

using namespace std;

unsigned int UIControl::SCR_WIDTH = 800;
unsigned int UIControl::SCR_HEIGHT = 600;

static Scene* s_scene = nullptr;
static std::function<MousePressControl> s_mousePressControl = [](int, double, double) {};
static std::function<MouseMoveControl> s_mouseMoveControl = [](double, double) {};
static std::function<WheelControl> s_WheelControl = [](double, double) {};

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    s_scene->resizeWindow(width, height);
}

static void mousePress(GLFWwindow* window, int button, int action, int mods)
{
    if (action == 1)
    {
        double xpos;
        double ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        s_mousePressControl(button, xpos, ypos);
    }
}

static void mouseMove(GLFWwindow* window, double xpos, double ypos)
{
    s_mouseMoveControl(xpos, ypos);
}

static void wheelEvent(GLFWwindow* window, double xoffset, double yoffset)
{
    s_WheelControl(xoffset, yoffset);
}

UIControl::UIControl(const char* title, int width, int height) 
    : m_title(title)
    , m_initWidth(width)
    , m_initHeight(height)
{
    m_keyboardControl = [](int) {};
}

UIControl::~UIControl()
{
    glfwTerminate();
}

void UIControl::setTitle(const std::string& title)
{
    m_title = title;
}

bool UIControl::initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_initWidth, m_initHeight, m_title.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwSetWindowPos(m_window, (1920 - m_initWidth) / 2, (1080 - m_initHeight) / 2);
    glfwMakeContextCurrent(m_window);
    // resize event 
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    switchMouseInputMethod(false);
    // wheel event
    glfwSetScrollCallback(m_window, wheelEvent);

    m_isInitialize = true;
    return true;
} 

void UIControl::setScene(Scene* scene)
{
    s_scene = scene;
}

void UIControl::setKeyboardControlCallBack(std::function<KeyboardControl> callback)
{
    m_keyboardControl = callback;
}

void UIControl::setMouseMoveControl(std::function<MouseMoveControl> callback)
{
    s_mouseMoveControl = callback;
}

void UIControl::setMousePressControl(std::function<MousePressControl> callback)
{
    s_mousePressControl = callback;
}

void UIControl::setWheelControl(std::function<WheelControl> callback)
{
    s_WheelControl = callback;
}

void UIControl::exec()
{
    if (!m_isInitialize)
        return;

    int width;
    int height;
    glfwGetWindowSize(m_window, &width, &height);
    s_scene->resizeWindow(width, height);

    while (!glfwWindowShouldClose(m_window))
    {
        processInput(m_window);
        s_scene->draw();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void UIControl::switchMouseInputMethod(bool bRotateMode)
{
    glfwSetCursorPosCallback(m_window, nullptr);
    glfwSetMouseButtonCallback(m_window, nullptr);
    if (bRotateMode)
    {
        // rotate mode
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(m_window, mouseMove);
    }
    else
    {
        // click mode
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetMouseButtonCallback(m_window, mousePress);
    }
}

void UIControl::processInput(GLFWwindow * window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // direction control
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_keyboardControl(0);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_keyboardControl(1);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_keyboardControl(2);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_keyboardControl(3);

    // switch input method
    static bool s_bCurIsRotate = false;
    static int s_count = 0;
    static bool s_controlPressedState = false;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        if (++s_count >= 10)
        {
            s_controlPressedState = true;
            s_count = 0;
        }
    }
    if (s_controlPressedState == true && 
        (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE ||
        glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE))
    {
        s_controlPressedState = false;
        s_bCurIsRotate = !s_bCurIsRotate;
        switchMouseInputMethod(s_bCurIsRotate);
    }
}
