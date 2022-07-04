//
// Created by paja on 7/3/22.
//

#include "Image.h"
#include <fstream>

Image::Image(uint32_t width, uint32_t height, unsigned char *data) : _width(width),
                                                                     _height(height),
                                                                     _channelCount(1),
                                                                     _name("Image") {

    _image = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajorBit>(height, width);

    for (uint32_t i = 0; i < _height; i++) {
        for (uint32_t j = 0; j < _width; j++) {
            // reverse Image
            _image(_height - 1 - i, j) = static_cast<int>(data[i * _width + j]);
        }
    }

    //     DEBUGGING
    //    std::ofstream out("image.ppm");
    //    out << "P5\n"
    //        << _width << ' ' << _height << "\n255\n";
    //    for (int i = 0; i < (int)_height; i++) {
    //        for (int j = 0; j < (int)_width; j++) {
    //            out << (unsigned char)_image(i, j);
    //        }
    //    }
}
