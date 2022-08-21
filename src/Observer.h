//
// Created by paja on 8/16/22.
//

#ifndef CV_OBSERVER_H
#define CV_OBSERVER_H

#include <memory>
#include <variant>
#include <vector>
#include "IObserver.h"
#include "InputEvents.h"

class Observer {
   public:
    void operator()(const InputEvent::Keyboard &rEvent);
    void operator()(const InputEvent::MouseButton &rEvent);
    void operator()(const InputEvent::MouseCursor &rEvent);
    void operator()(const InputEvent::MouseWheel &rEvent);
    void operator()(const InputEvent::WindowResize &rEvent);

    void HandleInputEvent();
    void RegisterObserver(const std::weak_ptr<IObserver> &spObserver, InputEvent::InputEventType eventType);
    void AddEvent(const InputEvent::IEvent &rEvent);

   private:
    std::vector<std::variant<InputEvent::MouseButton, InputEvent::MouseWheel, InputEvent::MouseCursor, InputEvent::Keyboard, InputEvent::WindowResize>> eventQueue;
    std::vector<std::weak_ptr<IObserver>> _keyboardObserver;
    std::vector<std::weak_ptr<IObserver>> _mouseButtonObserver;
    std::vector<std::weak_ptr<IObserver>> _mouseCursorObserver;
    std::vector<std::weak_ptr<IObserver>> _mouseWheelObserver;
    std::vector<std::weak_ptr<IObserver>> _windowResizeObserver;
};

#endif  // CV_OBSERVER_H
