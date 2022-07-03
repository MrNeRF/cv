#include "VideoStream.h"
#include <string>
#include "File.h"
#include "Image.h"

// save a frame into a .pgm file
//static void save_frame_as_jpeg(AVCodecContext *pCodecCtx, AVFrame *pFrame, int frameNbr);

bool VideoStream::initStream() {
    auto &rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("initializing all the containers, codecs and protocols.");

    _pFormatContext = avformat_alloc_context();
    if (_pFormatContext == nullptr) {
        rLogger.error("Memory could not be allocated for format context");
        return false;
    }

    _pFrame = av_frame_alloc();
    if (_pFrame == nullptr) {
        rLogger.error("Failed to allocate memory for AVFrame");
        return false;
    }

    _pPacket = av_packet_alloc();
    if (_pPacket == nullptr) {
        rLogger.error("Failed to allocate memory for AVPacket");
        return false;
    }

    return true;
}

void VideoStream::closeStream() {
    avformat_close_input(&_pFormatContext);
    av_packet_free(&_pPacket);
    av_frame_free(&_pFrame);
    avcodec_free_context(&_pCodecContext);
    _bStreamOpen = false;
}

VideoStream::~VideoStream() {
    closeStream();
}

bool VideoStream::Open(const File &rFile) {
    auto &rLogger = Logger::GetInstance().GetLogger();
    if (_bStreamOpen) {
        rLogger.warn("Stream was not closed");
        closeStream();
    }

    if (!initStream()) {
        rLogger.error("Stream could not be initialized.", rFile.GetFileName());
        return false;
    }

    // Open the file and read its header.
    if (avformat_open_input(&_pFormatContext, rFile.GetFilePath().c_str(), nullptr, nullptr) != 0) {
        rLogger.error("ERROR could not open stream file %s", rFile.GetFileName());
        return false;
    }

    // this function populates pFormatContext->streams
    if (avformat_find_stream_info(_pFormatContext, nullptr) < 0) {
        rLogger.error("ERROR could not get the stream info");
        return false;
    }

    // The codec to decode/encode the format
    const AVCodec *pCodec = nullptr;
    const AVCodecParameters *pCodecParameters = nullptr;
    if (getCodedAndParams(&pCodec, &pCodecParameters) < 0) {
         return false;
    }

    _pCodecContext = avcodec_alloc_context3(pCodec);
    if (_pCodecContext == nullptr) {
        rLogger.error("Failed to allocated memory for AVCodecContext"); return false;
    }

    // Fill the codec context based on the values from the supplied codec parameters
    if (avcodec_parameters_to_context(_pCodecContext, pCodecParameters) < 0) {
        rLogger.error("Failed to copy codec params to codec context");
        return false;
    }

    // Initialize the AVCodecContext to use the given AVCodec.
    if (avcodec_open2(_pCodecContext, pCodec, nullptr) < 0) {
        rLogger.error("Failed to open codec through avcodec_open2");
        return false;
    }

    // Prepare first packet
    _bStreamOpen = getValidFrame() == 0 ? true : false;
    return _bStreamOpen;
}

bool VideoStream::readPacket() {
    do {
        int retVal = av_read_frame(_pFormatContext, _pPacket);
        if (retVal < 0) {
            return false;
        }
    } while (_pPacket->stream_index != _videoStreamIndex);
    return true;
}

int VideoStream::decodePacket() {
    auto &rLogger = Logger::GetInstance().GetLogger();
    int retVal = avcodec_send_packet(_pCodecContext, _pPacket);
    if (retVal < 0) {
        switch (retVal) {
        case AVERROR(EAGAIN):
            rLogger.info("EAGAIN");
            break;
        case AVERROR_EOF:
            rLogger.info("AVERROR_EOF");
            break;
        case AVERROR(EINVAL):
            rLogger.info("EINVAL");
            break;
        case AVERROR(ENOMEM):
            rLogger.info("ENOMEM");
            break;
        }
        return retVal;
    }
    // Return decoded output data (into a frame) from a decoder
    retVal = avcodec_receive_frame(_pCodecContext, _pFrame);

    if (retVal < 0) {
        // logging("Error while receiving a frame from the decoder: %s", av_err2str(response));
        switch (retVal) {
        case AVERROR(EAGAIN):
            rLogger.info("EAGAIN");
            break;
        case AVERROR_EOF:
//            rLogger.info("AVERROR_EOF");
            break;
        case AVERROR(EINVAL):
            rLogger.info("EINVAL");
            break;
        case AVERROR_INPUT_CHANGED:
            rLogger.info("AVERROR_INPUT_CHANGED");
        }
        return retVal;
    }

    ASSERT(retVal == 0)
    return retVal;
}

int VideoStream::getValidFrame() {
    int retVal = -1;
    do {
        bool bHasPacket = readPacket();
        if (!bHasPacket) {
            ASSERT(bHasPacket);
            auto &rLogger = Logger::GetInstance().GetLogger();
            rLogger.error("No packages has been read!");
            av_packet_unref(_pPacket);
            closeStream();
            return -1;
        }
        retVal = decodePacket();
    } while (retVal == AVERROR(EAGAIN));

    return retVal;
}

void VideoStream::operator>>(Image &img) {
    auto &rLogger = Logger::GetInstance().GetLogger();
    img = Image(_pFrame->width, _pFrame->height, _pFrame->data[0]);
    av_packet_unref(_pPacket);

    // prepare next frame to know if there are more valid frames;
    if (getValidFrame() < 0) {
        rLogger.info("Closing stream!");
        closeStream();
    }
}

bool VideoStream::IsOpen() {
    return _bStreamOpen;
}

int VideoStream::getCodedAndParams(const AVCodec **pCodec, const AVCodecParameters **pCodecParameters) {
    auto &rLogger = Logger::GetInstance().GetLogger();
    // loop though all the streams to find codec and codec params
    for (uint32_t streamNb = 0; streamNb < _pFormatContext->nb_streams; streamNb++) {
        *pCodecParameters = _pFormatContext->streams[streamNb]->codecpar;
        *pCodec = avcodec_find_decoder((*pCodecParameters)->codec_id);

        if (pCodec == nullptr) {
            rLogger.error("Unsupported codec!");
            return -1;
        }
        // when the stream is a video we store its index, codec parameters and codec
        if ((*pCodecParameters)->codec_type == AVMEDIA_TYPE_VIDEO) {
            _videoStreamIndex = streamNb;
            break;
        }
    }

    if (_videoStreamIndex == -1) {
        rLogger.error("File does not contain a video stream!");
        return -1;
    }

    return 0;
}

// static void save_frame_as_jpeg(AVCodecContext *pCodecCtx, AVFrame *pFrame, int frameNbr) {
//     const AVCodec *pJPEGCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
//     auto &rLogger = Logger::GetInstance().GetLogger();
//     if (pJPEGCodec == nullptr) {
//         return;
//     }
//     AVCodecContext *pJPEGCtx = avcodec_alloc_context3(pJPEGCodec);
//     if (pJPEGCtx == nullptr) {
//         return;
//     }
//
//     pJPEGCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
//     pJPEGCtx->height = pFrame->height;
//     pJPEGCtx->width = pFrame->width;
//     pJPEGCtx->time_base = pCodecCtx->time_base;
//
//     if (avcodec_open2(pJPEGCtx, pJPEGCodec, nullptr) < 0) {
//         return;
//     }
//
//     if (avcodec_send_frame(pJPEGCtx, pFrame) != 0) {
//         return;
//     }
//     AVFrame *pJPGFrame = av_frame_alloc();
//     if (pJPGFrame == nullptr) {
//         return;
//     }
//
//     AVPacket packet = {.data = nullptr, .size = 0};
//     int retVal = avcodec_receive_packet(pJPEGCtx, &packet);
//     if (retVal != 0) {
//         switch (retVal) {
//         case AVERROR(EAGAIN):
//             rLogger.info("Output is not available in this state - user must try to send new input");
//             break;
//         case AVERROR_EOF:
//             rLogger.info("the decoder has been fully flushed, and there will be no more output frames");
//             break;
//         case AVERROR(EINVAL):
//             rLogger.info("codec not opened, or it is an encoder");
//             break;
//         case AVERROR_INPUT_CHANGED:
//             rLogger.info("current decoded frame has changed parameters");
//             break;
//         default:
//             rLogger.info("Something else!");
//         }
//         return;
//     }
//
//     // pFrame->pkt_size
//     FILE *JPEGFile;
//     char fileName[256];
//     sprintf(fileName, "img-%06d.jpg", frameNbr);
//     JPEGFile = fopen(fileName, "wb");
//     fwrite(packet.data, 1, packet.size, JPEGFile);
//     fclose(JPEGFile);
//
//     av_frame_free(&pJPGFrame);
//     avcodec_close(pJPEGCtx);
// }