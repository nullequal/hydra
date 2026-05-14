#include "core/input/sdl/device_list.hpp"

#include "core/input/sdl/controller.hpp"
#include "core/input/sdl/keyboard.hpp"
#include "frontend/sdl3/const.hpp"

namespace hydra::input::sdl {

void DeviceList::EventWatcher(SDL_Event* e) {
    switch (e->type) {
    case SDL_EVENT_KEYBOARD_ADDED: {
        if (kb_count == 0) {
            LOG_INFO(Input, "Keyboard connected: Generic Keyboard");
            devices.insert({"Generic Keyboard", new Keyboard()});
            kb_count++;
        } else {
            kb_count++;
        }
        break;
    }
    case SDL_EVENT_KEYBOARD_REMOVED: {
        if (kb_count == 1) {
            LOG_INFO(Input, "Keyboard disconnected: Generic Keyboard");
            auto it = devices.find("Generic Keyboard");
            ASSERT(it != devices.end(), Input, "Keyboard not connected");
            delete it->second;
            devices.erase(it);
            kb_count--;
        } else {
            kb_count--;
        }
        break;
    }
    case SDL_EVENT_GAMEPAD_ADDED: {
        SDL_Gamepad* gp = SDL_OpenGamepad(e->gdevice.which);
        std::string name = SDL_GetGamepadName(gp);
        LOG_INFO(Input, "Controller connected: {}", name);
        devices.insert({name, new Controller(gp)});
        break;
    }
    case SDL_EVENT_GAMEPAD_REMOVED: {
        SDL_Gamepad* gp = SDL_GetGamepadFromID(e->gdevice.which);
        std::string name = SDL_GetGamepadName(gp);
        LOG_INFO(Input, "Controller disconnected: {}", name);
        auto it = devices.find(name);
        ASSERT(it != devices.end(), Input, "Controller not connected");
        delete it->second;
        SDL_CloseGamepad(gp);
        devices.erase(it);
        break;
    }
    default:
        break;
    }
}

bool EWWrapper(void* userdata, SDL_Event* e) {
    auto o = static_cast<DeviceList*>(userdata);
    o->EventWatcher(e);
    return false;
}

DeviceList::DeviceList() { SDL_AddEventWatch(EWWrapper, this); }

DeviceList::~DeviceList() { SDL_RemoveEventWatch(EWWrapper, this); }

} // namespace hydra::input::sdl