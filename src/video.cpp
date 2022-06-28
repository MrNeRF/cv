//
// Created by paja on 6/27/22.
//

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavcodec/codec.h>
}


int main(int argc, char **argv) {

    AVCodecContext *dec_ctx;
    AVFrame *frame = new AVFrame;
    frame->width = 1;
    avcodec_receive_frame(dec_ctx, frame);

    return 0;
}
