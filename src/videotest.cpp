//
// Created by paja on 6/27/22.
//
#include "File.h"
#include "Image.h"
#include "VideoStream.h"

int main(int argc, const char *argv[]) {

    VideoStream stream;
    File file = File("output.mp4", File::FileType::Video);
    stream.Open(file);
    while (stream.IsOpen()) {
        Image img;
        stream >> img;
    }
    return 0;
}