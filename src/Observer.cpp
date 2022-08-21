//
// Created by paja on 8/16/22.
//

#include "Observer.h"

void Observer::operator()(const InputEvent::MouseButton &rEvent) {
    for (std::weak_ptr<IObserver> &spObserver : _mouseButtonObserver) {
        if (!spObserver.expired()) {
            auto sp = spObserver.lock();
            (*sp).Update(rEvent);
        }
    }
}

void Observer::operator()(const InputEvent::MouseWheel &rEvent) {
    for (std::weak_ptr<IObserver> &spObserver : _mouseWheelObserver) {
        if (!spObserver.expired()) {
            auto sp = spObserver.lock();
            (*sp).Update(rEvent);
        }
    }
}

void Observer::operator()(const InputEvent::MouseCursor &rEvent) {
    for (std::weak_ptr<IObserver> &spObserver : _mouseCursorObserver) {
        if (!spObserver.expired()) {
            auto sp = spObserver.lock();
            (*sp).Update(rEvent);
        }
    }
}

void Observer::operator()(const InputEvent::Keyboard &rEvent) {
    for (std::weak_ptr<IObserver> &spObserver : _keyboardObserver) {
        if (!spObserver.expired()) {
            auto sp = spObserver.lock();
            (*sp).Update(rEvent);
        }
    }
}

void Observer::operator()(const InputEvent::WindowResize &rEvent) {
    for (std::weak_ptr<IObserver> &spObserver : _windowResizeObserver) {
        if (!spObserver.expired()) {
            auto sp = spObserver.lock();
            (*sp).Update(rEvent);
        }
    }
}

void Observer::HandleInputEvent() {
    for (auto &rEvent : eventQueue) {
        std::visit(*this, rEvent);
    }

    eventQueue.clear();
}

void Observer::RegisterObserver(const std::weak_ptr<IObserver> &spObserver, InputEvent::InputEventType eventType) {
    switch (eventType) {
    case InputEvent::InputEventType::KeyBoard:
        _keyboardObserver.push_back(spObserver);
        break;
    case InputEvent::InputEventType::MouseButton:
        _mouseButtonObserver.push_back(spObserver);
        break;
    case InputEvent::InputEventType::MouseCursor:
        _mouseCursorObserver.push_back(spObserver);
        break;
    case InputEvent::InputEventType::MouseWheel:
        _mouseWheelObserver.push_back(spObserver);
        break;
    case InputEvent::InputEventType::WindowResize:
        _windowResizeObserver.push_back(spObserver);
        break;
    }
}

void Observer::AddEvent(const InputEvent::IEvent &rEvent) {
    switch (rEvent.eventType) {
    case InputEvent::InputEventType::KeyBoard:
        eventQueue.emplace_back(dynamic_cast<const InputEvent::Keyboard &>(rEvent));
        break;
    case InputEvent::InputEventType::MouseButton:
        eventQueue.emplace_back(dynamic_cast<const InputEvent::MouseButton &>(rEvent));
        break;
    case InputEvent::InputEventType::MouseCursor:
        eventQueue.emplace_back(dynamic_cast<const InputEvent::MouseCursor &>(rEvent));
        break;
    case InputEvent::InputEventType::MouseWheel:
        eventQueue.emplace_back(dynamic_cast<const InputEvent::MouseWheel &>(rEvent));
        break;
    case InputEvent::InputEventType::WindowResize:
        eventQueue.emplace_back(dynamic_cast<const InputEvent::WindowResize &>(rEvent));
        break;
    }
}