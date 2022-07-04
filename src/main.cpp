#include "File.h"
#include "Image.h"
#include "ImageViewer.h"
#include "VideoStream.h"

int main() {
    auto viewer = ImageViewer("ImageViewer Window");
    File file = File("output.mp4", File::FileType::Video);

    if (viewer.Init() < 0) {
        return -1;
    }

    VideoStream stream;
    stream.Open(file);
    while (stream.IsOpen()) {
        Image img;
        stream >> img;
        viewer.ShowImage(img);
    }

    return 0;
}
