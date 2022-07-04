//
// Created by paja on 7/3/22.
//

#ifndef CV_IMAGE_H
#define CV_IMAGE_H

#include <Eigen/Dense>

class Image {
   private:
   public:
    Image() = default;
    explicit Image(uint32_t width, uint32_t height, unsigned char* data);
    const std::string& GetName() const { return _name; };
    const Eigen::MatrixXi& GetImageData() const { return _image; };
    uint32_t GetWidth() const { return _width; }
    uint32_t GetHeight() const { return _height; }
    uint8_t GetChannelCount() const { return _channelCount; };

   private:
    uint32_t _width;
    uint32_t _height;
    uint8_t _channelCount;
    std::string _name;
    Eigen::MatrixXi _image;
};

#endif  // CV_IMAGE_H
