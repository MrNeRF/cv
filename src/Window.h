#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include <chrono>
#include <cstdint>
#include <list>
#include <ratio>
#include <string>
#include "GLFW/glfw3.h"

class Window {
    using seconds = std::chrono::duration<double>;
    using milliseconds = std::chrono::duration<double, std::ratio_multiply<seconds::period, std::milli>>;

   public:
    explicit Window(const std::string& name) : Window{name, 800, 800} {}
    explicit Window(const std::string& name, int height, int width) : _windowName{name},
                                                                      _winHeight{height},
                                                                      _winWidth{width} {}
    ~Window(void);

    int Init();
    // Callbacks
    static void WindowResizeCallback(GLFWwindow* win, int h, int w);
    static void MouseInputCallback(GLFWwindow* win, int button, int action, int mods);
    static void MouseWheelCallback(GLFWwindow* win, double xoffset, double yoffset);
    static void CursorPositionCallback(GLFWwindow* win, double xCursorPos, double yCursorPos);
    static void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods);

    GLFWwindow* GetGLFWWindow(void) { return _windowInstance; }

    // Associated class methods invoked by registered callbacks.
    void ViewPortUpdate(int width, int height);
    void MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods);
    void MouseWheelUpdate(double xoffset, double yoffset);
    void CursorPositionUpdate(double xCursorPos, double yCursorPos);
    void KeyboardDeviceUpdate(int key, int scancode, int action, int mods);

    // ...
    float GetAspectRatio() { return static_cast<float>(std::max(_winHeight, _winWidth)) / static_cast<float>(std::min(_winHeight, _winWidth)); }

   public:
    const std::string _windowName;
    int _winHeight;
    int _winWidth;
    int _key = 0;

   private:
    GLFWwindow* _windowInstance = nullptr;
};

#endif
