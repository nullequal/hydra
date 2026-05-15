#pragma once

#include "core/input/device_list.hpp"

namespace hydra::input::apple_gc {

class DeviceList : public IDeviceList {
  public:
    DeviceList();
    ~DeviceList();

  private:
    id impl;
};

} // namespace hydra::input::apple_gc
