#pragma once

#include "core/input/const.hpp"

namespace hydra::input {

struct NpadState {
    horizon::services::hid::NpadButtons buttons{
        horizon::services::hid::NpadButtons::None};
    float2 analog_l;
    float2 analog_r;
};

struct TouchState {
    u32 x;
    u32 y;
    // TODO: more
};

} // namespace hydra::input
