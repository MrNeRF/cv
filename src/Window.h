#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include <GLFW/glfw3.h>
#include <Observer.h>
#include <Eigen/Dense>
#include <chrono>
#include <cstdint>
#include <list>
#include <ratio>
#include <string>

class Window : public Observer {
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

    Eigen::Vector3f GetLastDirection() {
        auto tmp = _lastDirection;
        _lastDirection = Eigen::Vector3f(0.f, 0.f, 0.f);
        return tmp;
    }

    Eigen::Vector2d GetCursorPostionDelta();

    GLFWwindow* GetGLFWWindow(void) { return _windowInstance; }

    void ViewPortUpdate(int width, int height);
    void MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods);
    void CursorPositionUpdate(double xCursorPos, double yCursorPos);
    void KeyboardDeviceUpdate(int key, int scancode, int action, int mods);

    // ...
    float GetAspectRatio() { return static_cast<float>(std::max(_winHeight, _winWidth)) / static_cast<float>(std::min(_winHeight, _winWidth)); }

   public:
    Eigen::Vector3f _lastDirection = Eigen::Vector3f(0.f, 0.f, 0.f);
    Eigen::Vector2d _lastCursorPostion;
    Eigen::Vector2d _deltaCursorPosition;
    bool _updateMousePostion = false;
    const std::string _windowName;
    int _winHeight;
    int _winWidth;
    int _key = 0;

   private:
    GLFWwindow* _windowInstance = nullptr;
};

#endif
