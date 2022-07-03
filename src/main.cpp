#include "File.h"
#include "Image.h"
#include "VideoStream.h"
#include "Viewer3D.h"

int main() {

    auto viewer = Viewer3D("3D Viewer3D Window");
    VideoStream stream;
    File file = File("output.mp4", File::FileType::Video);
    stream.Open(file);
    while (stream.IsOpen()) {
        Image img;
        stream >> img;
    }
    return 0;

    if (viewer.Init() < 0) {
        return -1;
    }

    viewer.SetBackgroundColor({0.45f, 0.55f, 0.60f, 1.00f});
    viewer.Run();

    return 0;
}
