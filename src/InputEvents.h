//
// Created by paja on 8/16/22.
//

#ifndef CV_INPUTEVENTS_H
#define CV_INPUTEVENTS_H

namespace InputEvent {
    enum class InputEventType {
        MouseButton,
        MouseWheel,
        MouseCursor,
        KeyBoard
    };

    struct IEvent {
        explicit IEvent(InputEventType eventType) : eventType{eventType} {}
        virtual ~IEvent() = default;
        InputEventType eventType;
    };

    struct MouseButton : public IEvent {
        MouseButton() : IEvent(InputEventType::MouseButton) {}
        int button;
        int action;
        int mods;
    };

    struct MouseWheel : public IEvent {
        MouseWheel() : IEvent(InputEventType::MouseWheel) {}
        double xOffset;
        double yOffeset;
    };

    struct MouseCursor : public IEvent {
        MouseCursor() : IEvent(InputEventType::MouseCursor) {}
        double xCursorPos;
        double yCursorPos;
    };

    struct Keyboard : public IEvent {
        Keyboard() : IEvent(InputEventType::KeyBoard) {}
        int key;
        int scancode;
        int action;
        int mods;
    };
}  // namespace InputEvent

#endif  // CV_INPUTEVENTS_H
