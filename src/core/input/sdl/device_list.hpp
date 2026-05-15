#pragma once

#include "core/input/device_list.hpp"
#include "frontend/sdl3/const.hpp"

namespace hydra::input::sdl {

class DeviceList : public IDeviceList {
  public:
    DeviceList();
    ~DeviceList();

    void EventWatcher(SDL_Event* e);

  private:
    u32 keyboard_count;

    // Helpers
    void ConnectGenericKeyboard();
    void ConnectController(SDL_JoystickID id);
};

} // namespace hydra::input::sdl
