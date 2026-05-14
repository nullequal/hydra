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
    u32 kb_count;
};

} // namespace hydra::input::sdl
