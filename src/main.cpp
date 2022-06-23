#include "Viewer.h"
#include "Window.h"
#include <memory>

int main() {

    auto viewer = Viewer(std::make_unique<Window>("3D Viewer Window"));

    if (viewer.Init() < 0) {
        return -1;
    }

    viewer.SetBackgroundColor({0.45f, 0.55f, 0.60f, 1.00f});
    viewer.Run();

    return 0;
}
