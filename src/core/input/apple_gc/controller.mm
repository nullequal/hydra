#include "core/input/apple_gc/controller.hpp"

#import <GameController/GameController.h>

namespace hydra::input::apple_gc {

bool Controller::IsPressedImpl(ControllerInput input) {
#define BUTTON_CASE(input, gc_button)                                          \
    case ControllerInput::input:                                               \
        gc_button_name = GCInput##gc_button;                                   \
        break;

    NSString<GCButtonElementName>* gc_button_name = nil;
    switch (input) {
        BUTTON_CASE(A, ButtonB)
        BUTTON_CASE(B, ButtonA)
        BUTTON_CASE(X, ButtonY)
        BUTTON_CASE(Y, ButtonX)
        BUTTON_CASE(StickL, LeftThumbstickButton)
        BUTTON_CASE(StickR, RightThumbstickButton)
        BUTTON_CASE(L, LeftBumper)
        BUTTON_CASE(R, RightBumper)
        BUTTON_CASE(ZL, LeftTrigger)
        BUTTON_CASE(ZR, RightTrigger)
    // TODO: implement
    case ControllerInput::Plus:
    case ControllerInput::Minus:
    case ControllerInput::Left:
    case ControllerInput::Up:
    case ControllerInput::Right:
    case ControllerInput::Down:
    case ControllerInput::LeftSL:
    case ControllerInput::LeftSR:
    case ControllerInput::RightSL:
    case ControllerInput::RightSR:
        break;
    default:
        LOG_NOT_IMPLEMENTED(Input, "Controller button {}", input);
        return false;
    }

#undef BUTTON_CASE

    return reinterpret_cast<GCController*>(handle)
        .physicalInputProfile.buttons[gc_button_name]
        .isPressed;
}

i32 analog_stick_to_int(f32 value) {
    if (value < 0.0f)
        return std::max(i16(value * (-std::numeric_limits<i16>::min())),
                        std::numeric_limits<i16>::min());
    else
        return std::min(i16(value * std::numeric_limits<i16>::max()),
                        std::numeric_limits<i16>::max());
}

i32 Controller::GetAxisValueImpl(ControllerInput input) {
#define AXIS_CASE(input, gc_dpad, direction)                                   \
    case ControllerInput::input:                                               \
        gc_dpad_name = GCInput##gc_dpad;                                       \
        dir = AnalogStickDirection::direction;                                 \
        break;

    NSString<GCDirectionPadElementName>* gc_dpad_name = nil;
    AnalogStickDirection dir;
    switch (input) {
        AXIS_CASE(StickLLeft, LeftThumbstick, Left)
        AXIS_CASE(StickLUp, LeftThumbstick, Up)
        AXIS_CASE(StickLRight, LeftThumbstick, Right)
        AXIS_CASE(StickLDown, LeftThumbstick, Down)
        AXIS_CASE(StickRLeft, RightThumbstick, Left)
        AXIS_CASE(StickRUp, RightThumbstick, Up)
        AXIS_CASE(StickRRight, RightThumbstick, Right)
        AXIS_CASE(StickRDown, RightThumbstick, Down)
    default:
        LOG_NOT_IMPLEMENTED(Input, "Controller axis {}", input);
        return 0;
    }

#undef AXIS_CASE

    auto dpad = reinterpret_cast<GCController*>(handle)
                    .physicalInputProfile.dpads[gc_dpad_name];
    switch (dir) {
    case AnalogStickDirection::Right:
        return analog_stick_to_int(dpad.right.value);
    case AnalogStickDirection::Left:
        return analog_stick_to_int(dpad.left.value);
    case AnalogStickDirection::Up:
        return analog_stick_to_int(dpad.up.value);
    case AnalogStickDirection::Down:
        return analog_stick_to_int(dpad.down.value);
    }
}

} // namespace hydra::input::apple_gc
