#include <GL/glew.h>
//
#include <iostream>
#include "Logger.h"
#include "Window.h"

static void APIENTRY glDebugOutput(GLenum source,
                                   GLenum type,
                                   unsigned int id,
                                   GLenum severity,
                                   GLsizei length,
                                   const char* message,
                                   const void* userParam) {
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;  // ignore these non-significant error codes

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

int Window::Init() {
    // initialise glfw and _windowInstance,
    // create openGL context, initialise any other c++ resources
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    _windowInstance = glfwCreateWindow(_winWidth, _winHeight, _windowName.c_str(), NULL, NULL);

    auto& rLogger = Logger::GetInstance().GetLogger();
    if (_windowInstance == NULL) {
        rLogger.error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(_windowInstance);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        rLogger.error("Failed to initialize GLEW");
        glfwTerminate();
        return -1;
    }
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);  // makes sure errors are displayed synchronously
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    // glewInit() seems to pollute OpenGL with error codes
    int errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        rLogger.trace("Polluted by glewInit error: {}. Just ignore...", errorCode);
    }
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
    const GLubyte* version = glGetString(GL_VERSION);    // version as a string
    rLogger.info("Renderer: {}, OpenGL version supported: {}", renderer, version);
    // Userpointer -> Necessary to for casting
    glfwSetWindowUserPointer(_windowInstance, this);

    // Register Callbacks
    glfwSetFramebufferSizeCallback(_windowInstance, WindowResizeCallback);
    glfwSetMouseButtonCallback(_windowInstance, MouseInputCallback);
    glfwSetScrollCallback(_windowInstance, MouseWheelCallback);
    glfwSetCursorPosCallback(_windowInstance, CursorPositionCallback);
    glfwSetKeyCallback(_windowInstance, KeyboardCallback);
    return 0;
}

Window::~Window(void) {
    glfwDestroyWindow(_windowInstance);
    glfwTerminate();
}

void Window::ViewPortUpdate(int width, int height) {
    _winWidth = width;
    _winHeight = height;
    CHECK_GL_ERROR_(glViewport(0, 0, static_cast<GLsizei>(_winWidth), static_cast<GLsizei>(_winHeight)))
    CHECK_GL_ERROR_(glMatrixMode(GL_PROJECTION))
    CHECK_GL_ERROR_(glLoadIdentity())
    CHECK_GL_ERROR_(glMatrixMode(GL_MODELVIEW))
}

void Window::WindowResizeCallback(GLFWwindow* win, int h, int w) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->ViewPortUpdate(w, h);
}

void Window::MouseInputCallback(GLFWwindow* win, int button, int action, int mods) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->MouseDeviceUpdate(win, button, action, mods);
}

void Window::MouseWheelCallback(GLFWwindow* win, double xoffset, double yoffset) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->MouseWheelUpdate(xoffset, yoffset);
}

void Window::CursorPositionCallback(GLFWwindow* win, double xCursorPos, double yCursorPos) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->CursorPositionUpdate(xCursorPos, yCursorPos);
}

void Window::KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
    window->KeyboardDeviceUpdate(key, scancode, action, mods);
}

void Window::MouseDeviceUpdate(GLFWwindow* win, int button, int action, int mods) {
}

void Window::KeyboardDeviceUpdate(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
    } else if (action == GLFW_RELEASE) {
    }

    _key = key;

    if (action == GLFW_PRESS) {
        switch (_key) {
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(_windowInstance, true);
            break;
        }
    }
}

void Window::MouseWheelUpdate(double xoffset, double yoffset) {
}

void Window::CursorPositionUpdate(double xCursorPos, double yCursorPos) {
}
