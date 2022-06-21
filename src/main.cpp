#include <Viewer.h>
#include <Window.h>

int main()
{
    Viewer  viewer;
    Window* pWindow = viewer.Init("MyWindow");
    if (pWindow == nullptr)
    {
        return -1;
    }

    viewer.Run();
    return 0;
}
