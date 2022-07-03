//
// Created by paja on 7/3/22.
//

#ifndef CV_IMAGE_H
#define CV_IMAGE_H

#include <Eigen/Dense>

class Image {
   public:
    Image() = default;
    explicit Image(uint32_t width, uint32_t height, unsigned char* data);

   private:
    Eigen::MatrixXi _image;
};

#endif  // CV_IMAGE_H
