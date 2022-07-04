//
// Created by paja on 7/3/22.
//

#ifndef CV_VIEWER_H
#define CV_VIEWER_H

class Viewer {
   public:
    virtual ~Viewer() = default;
    virtual int Init() = 0;
};

#endif  // CV_VIEWER_H
