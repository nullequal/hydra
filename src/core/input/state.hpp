#pragma once

#include "core/input/const.hpp"

namespace hydra::input {

struct NpadState {
    horizon::services::hid::NpadButtons buttons{
        horizon::services::hid::NpadButtons::None};
    i32 analog_l_x;
    i32 analog_l_y;
    i32 analog_r_x;
    i32 analog_r_y;
};

struct TouchState {
    u32 x;
    u32 y;
    // TODO: more
};

} // namespace hydra::input
