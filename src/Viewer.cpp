#include "Viewer.h"
#include "File.h"
#include "Logger.h"
#include "Shader.h"
#include "Window.h"
#include <Eigen/Dense>
#include <GLFW/glfw3.h>
#include <memory>

Window* Viewer::Init(const std::string& name)
{
    m_spWindow = std::make_unique<Window>(name);
    ASSERT(m_spWindow);
    return m_spWindow.get();
}

void Viewer::Run(void)
{
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Viewer::Run()");
    if (m_spWindow == nullptr)
    {
        rLogger.critical("No Window Object created!");
        ASSERT(0);
    }

    CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST));
    Eigen::Vector4f clear_color(0.45f, 0.55f, 0.60f, 1.00f);

    std::vector<std::string> filenames = File("Shader").GetDirectoryContents();
    std::vector<const char*> items;
    std::transform(std::begin(filenames), std::end(filenames), std::back_inserter(items), std::mem_fn(&std::string::c_str));
    while (!glfwWindowShouldClose(m_spWindow->GetGLFWWindow()))
    {
        glfwPollEvents();

        CHECK_GL_ERROR_(glClearColor(clear_color.x() * clear_color.w(), clear_color.y() * clear_color.w(), clear_color.z() * clear_color.w(), clear_color.w()));
        CHECK_GL_ERROR_(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

        CHECK_GL_ERROR_(glEnable(GL_DEPTH_TEST))
        CHECK_GL_ERROR_(glDepthFunc(GL_LESS))
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glfwSwapBuffers(m_spWindow->GetGLFWWindow());
    }
}
