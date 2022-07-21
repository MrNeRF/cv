#include "Viewer3D.h"

int main() {
    auto viewer = Viewer3D("My Window");

    if (viewer.Init() < 0) {
        return -1;
    }

    viewer.SetBackgroundColor(Eigen::Vector4f(0.1f, 0.2f, .5f, 1.f));
    viewer.Run();

    return 0;
}
