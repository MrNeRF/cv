//
// Created by paja on 6/27/22.
//

extern "C" {
#include <libavcodec/avcodec.h>
}

int main() {
    RcOverride rc;
    rc.end_frame = 1;
    rc.start_frame = 1;
}