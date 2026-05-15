#include "core/input/sdl/device_list.hpp"

#include "core/input/sdl/controller.hpp"
#include "core/input/sdl/keyboard.hpp"
#include "frontend/sdl3/const.hpp"

namespace hydra::input::sdl {

namespace {

bool EWWrapper(void* userdata, SDL_Event* e) {
    auto o = static_cast<DeviceList*>(userdata);
    o->EventWatcher(e);
    return false;
}

} // namespace

DeviceList::DeviceList() { SDL_AddEventWatch(EWWrapper, this); }

DeviceList::~DeviceList() { SDL_RemoveEventWatch(EWWrapper, this); }

void DeviceList::EventWatcher(SDL_Event* e) {
    switch (e->type) {
    case SDL_EVENT_KEYBOARD_ADDED: {
        if (kb_count++ == 0)
            AddDevice("Generic Keyboard", new Keyboard());
        kb_count++;
        break;
    }
    case SDL_EVENT_KEYBOARD_REMOVED: {
        if (--kb_count == 0)
            RemoveDevice("Generic Keyboard");
        break;
    }
    case SDL_EVENT_GAMEPAD_ADDED: {
        SDL_Gamepad* gp = SDL_OpenGamepad(e->gdevice.which);
        std::string name = SDL_GetGamepadName(gp);
        AddDevice(name, new Controller(gp));
        break;
    }
    case SDL_EVENT_GAMEPAD_REMOVED: {
        SDL_Gamepad* gp = SDL_GetGamepadFromID(e->gdevice.which);
        std::string name = SDL_GetGamepadName(gp);
        RemoveDevice(name);
        break;
    }
    default:
        break;
    }
}

} // namespace hydra::input::sdl
