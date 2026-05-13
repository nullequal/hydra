#include "core/input/sdl/device_list.hpp"

#include "core/input/sdl/controller.hpp"
#include "core/input/sdl/keyboard.hpp"
#include "frontend/sdl3/const.hpp"

namespace hydra::input::sdl {

bool EventWatcher(void* userdata, SDL_Event* e) {
    switch (e->type) {
    case SDL_EVENT_GAMEPAD_ADDED: {
        SDL_Gamepad* gp = SDL_OpenGamepad(e->gdevice.which);
        std::string name = SDL_GetGamepadName(gp);
        LOG_INFO(Input, "Controller connected: {}", name);
        auto devices = static_cast<std::map<std::string, IDevice*>*>(userdata);
        devices->insert({name, new Controller(gp)});
        break;
    }
    case SDL_EVENT_GAMEPAD_REMOVED: {
        SDL_Gamepad* gp = SDL_GetGamepadFromID(e->gdevice.which);
        std::string name = SDL_GetGamepadName(gp);
        LOG_INFO(Input, "Controller disconnected: {}", name);
        auto devices = static_cast<std::map<std::string, IDevice*>*>(userdata);
        auto it = devices->find(name);
        ASSERT(it != devices->end(), Input, "Controller not connected");
        delete it->second;
        SDL_CloseGamepad(gp);
        devices->erase(it);
    }
    }
    return false;
}

DeviceList::DeviceList() {
    // TODO: more than one keyboard
    devices["keyboard"] = new Keyboard();
    LOG_INFO(Input, "Keyboard connected: keyboard");
    SDL_AddEventWatch(EventWatcher, &devices);
}

DeviceList::~DeviceList() {
    SDL_RemoveEventWatch(EventWatcher, &devices);
}

} // namespace hydra::input::sdl