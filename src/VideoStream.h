#include <cinttypes>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Logger.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

class Image;
class File;

class VideoStream {
   public:
    ~VideoStream();
    bool Open(const File &rFile);
    bool IsOpen();
    void operator>>(Image &img);

   private:
    int getCodedAndParams(const AVCodec **pCodec, const AVCodecParameters **pCodecParameters);
    int getValidFrame();
    bool readPacket();
    int decodePacket();
    bool initStream();
    void closeStream();

   private:
    int _videoStreamIndex = -1;
    bool _bStreamOpen = false;
    AVFormatContext *_pFormatContext;
    AVFrame *_pFrame;
    AVPacket *_pPacket;
    AVCodecContext *_pCodecContext;
};