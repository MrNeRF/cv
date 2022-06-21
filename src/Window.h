#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "GLFW/glfw3.h"
#include <chrono>
#include <cstdint>
#include <list>
#include <ratio>
#include <string>

class Window
{
    using seconds      = std::chrono::duration<double>;
    using milliseconds = std::chrono::duration<double, std::ratio_multiply<seconds::period, std::milli>>;

public:
    Window(const std::string name);
    ~Window(void);

    // Callbacks
    static void WindowResizeCallback(GLFWwindow* win, int h, int w);
    static void MouseInputCallback(GLFWwindow* win, int button, int action, int mods);
    static void MouseWheelCallback(GLFWwindow* win, double xoffset, double yoffset);
    static void CursorPositionCallback(GLFWwindow* win, double xCursorPos, double yCursorPos);
    static void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods);

    GLFWwindow* GetGLFWWindow(void) { return m_windowInstance; }

    // Associated class methods invoked by registered callbacks.
    void ViewPortUpdate(int width, int height);
    void MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods);
    void MouseWheelUpdate(double xoffset, double yoffset);
    void CursorPositionUpdate(double xCursorPos, double yCursorPos);
    void KeyboardDeviceUpdate(int key, int scancode, int action, int mods);

    // ...
    float GetAspectRatio() { return static_cast<float>(std::max(winHeight, winWidth)) / static_cast<float>(std::min(winHeight, winWidth)); }

public:
    int               winHeight = 800;
    int               winWidth  = 800;
    const std::string windowName;
    bool              m_bKeyPressed = false;
    int               m_key         = 0;
    bool              m_bWireFrame  = false;

private:
    GLFWwindow* m_windowInstance = nullptr;
};

#endif
