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

DeviceList::DeviceList() {
    // Get initial keyboards
    int kb_count = 0;
    SDL_KeyboardID* keyboards = SDL_GetKeyboards(&kb_count);
    if (keyboards) {
        keyboard_count = static_cast<u32>(kb_count);
        if (keyboard_count > 0)
            ConnectGenericKeyboard();

        SDL_free(keyboards);
    } else {
        keyboard_count = 0;
    }

    // Get initial gamepads
    int gp_count = 0;
    SDL_JoystickID* gamepads = SDL_GetGamepads(&gp_count);
    if (gamepads) {
        for (int i = 0; i < gp_count; i++)
            ConnectController(gamepads[i]);

        SDL_free(gamepads);
    }

    // Register event watcher
    SDL_AddEventWatch(EWWrapper, this);
}

DeviceList::~DeviceList() { SDL_RemoveEventWatch(EWWrapper, this); }

void DeviceList::EventWatcher(SDL_Event* e) {
    switch (e->type) {
    case SDL_EVENT_KEYBOARD_ADDED: {
        if (keyboard_count++ == 0)
            ConnectGenericKeyboard();
        keyboard_count++;
        break;
    }
    case SDL_EVENT_KEYBOARD_REMOVED: {
        if (--keyboard_count == 0)
            RemoveDevice("Generic Keyboard");
        break;
    }
    case SDL_EVENT_GAMEPAD_ADDED: {
        ConnectController(e->gdevice.which);
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

void DeviceList::ConnectGenericKeyboard() {
    AddDevice("Generic Keyboard", new Keyboard());
}

void DeviceList::ConnectController(SDL_JoystickID id) {
    SDL_Gamepad* gp = SDL_OpenGamepad(id);
    if (gp) {
        std::string name = SDL_GetGamepadName(gp);
        AddDevice(name, new Controller(gp));
    } else {
        LOG_ERROR(Input, "Failed to get controller: {}", SDL_GetError());
    }
}

} // namespace hydra::input::sdl
