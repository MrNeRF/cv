//
// Created by paja on 8/16/22.
//

#ifndef CV_IOBSERVER_H
#define CV_IOBSERVER_H

#include "InputEvents.h"

class IObserver {
   public:
    virtual ~IObserver() = default;
    virtual void Update(const InputEvent::IEvent &rEvent) = 0;
};

#endif  // CV_IOBSERVER_H
