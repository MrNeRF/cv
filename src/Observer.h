//
// Created by paja on 8/16/22.
//

#ifndef CV_OBSERVER_H
#define CV_OBSERVER_H

#include <memory>
#include <variant>
#include <vector>
#include "InputEvents.h"
#include "IObserver.h"

class Observer {
   public:
    void operator()(const InputEvent::MouseButton &rEvent);
    void operator()(const InputEvent::MouseWheel &rEvent);
    void operator()(const InputEvent::MouseCursor &rEvent);
    void operator()(const InputEvent::Keyboard &rEvent);

    void HandleInputEvent();
    void RegisterObserver(const std::weak_ptr<IObserver>& spObserver, InputEvent::InputEventType eventType);
    void AddEvent(const InputEvent::IEvent &rEvent);

   private:
    std::vector<std::variant<InputEvent::MouseButton, InputEvent::MouseWheel, InputEvent::MouseCursor, InputEvent::Keyboard>> eventQueue;
    std::vector<std::weak_ptr<IObserver>> mouseButtonObserver;
    std::vector<std::weak_ptr<IObserver>> mouseWheelObserver;
    std::vector<std::weak_ptr<IObserver>> mouseCursorObserver;
    std::vector<std::weak_ptr<IObserver>> keyboardObserver;
};

#endif  // CV_OBSERVER_H