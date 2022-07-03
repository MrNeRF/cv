//
// Created by paja on 7/3/22.
//

#include "Image.h"
Image::Image(uint32_t width, uint32_t height, unsigned char *data) {
    _image = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajorBit>(height, width);
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            _image(i, j) = static_cast<int>(data[i * height + j]);
        }
    }
}
