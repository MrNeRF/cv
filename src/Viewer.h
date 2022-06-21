#ifndef VIEWER_H
#define VIEWER_H

#include <memory>
#include <string>
#include <vector>

class Window;

class Viewer
{
public:
    Window* Init(const std::string& name);
    void    Run(void);

private:
    std::unique_ptr<Window> m_spWindow;
};

#endif
