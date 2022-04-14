////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/WindowStyle.hpp> // important to be included first (conflict with None)
#include <SFML/Window/Switch/WindowImplSwitch.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <switch.h>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace sf
{
namespace priv
{
WindowImplSwitch* WindowImplSwitch::singleInstance = NULL;

////////////////////////////////////////////////////////////
WindowImplSwitch::WindowImplSwitch(WindowHandle handle)
{
    singleInstance = this;
}


////////////////////////////////////////////////////////////
WindowImplSwitch::WindowImplSwitch(VideoMode mode, const String& title, unsigned long style, const ContextSettings& settings)
{
    singleInstance = this;

    Result res = viInitialize(ViServiceType_Application);
    if (R_FAILED(res))
    {
        err() << "viInitialize failed" << std::endl;
    }
    hidInitializeTouchScreen(); // Init touch screen

    NWindow* defaultPtr = nwindowGetDefault();
    m_window = *defaultPtr;
}


////////////////////////////////////////////////////////////
WindowImplSwitch::~WindowImplSwitch()
{
    viExit();
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplSwitch::getSystemHandle() const
{
    return (WindowHandle) &m_window;
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::processEvents()
{
    appletMainLoop();
    processTouchscreenEvent();
}


////////////////////////////////////////////////////////////
Vector2i WindowImplSwitch::getPosition() const
{
    // Not applicable
    return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setPosition(const Vector2i& position)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
Vector2u WindowImplSwitch::getSize() const
{
    // TODO: Map to display size?
    return Vector2u(1280, 720);
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setSize(const Vector2u& size)
{
    // N/A
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setTitle(const String& title)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setVisible(bool visible)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setMouseCursorVisible(bool visible)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setMouseCursorGrabbed(bool grabbed)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setMouseCursor(const CursorImpl& cursor)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setKeyRepeatEnabled(bool enabled)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::requestFocus()
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool WindowImplSwitch::hasFocus() const
{
    // TODO: use libnx somehow?
    return true;
}

void WindowImplSwitch::forwardEvent(const Event& event)
{
    WindowImplSwitch::singleInstance->pushEvent(event);
}

void WindowImplSwitch::processTouchscreenEvent()
{
    static HidTouchScreenState previous_state = {0};

    HidTouchScreenState state={0};
    if (hidGetTouchScreenStates(&state, 1)) {
        for(s32 i=0; i < state.count; i++) {
            bool found = false;
            for (s32 y = 0; y < previous_state.count; y++) {
                if (state.touches[i].finger_id == previous_state.touches[y].finger_id) {
                    found = true;
                    if (state.touches[i].x != previous_state.touches[y].x || state.touches[i].y != previous_state.touches[y].y) {
                        Event evt;
                        evt.type = Event::TouchMoved;
                        evt.touch.finger = state.touches[i].finger_id;
                        evt.touch.x = state.touches[i].x;
                        evt.touch.y = state.touches[i].y;
                        forwardEvent(evt);
                    }
                }
            }
            if (!found) {
                Event evt;
                evt.type = Event::TouchBegan;
                evt.touch.finger = state.touches[i].finger_id;
                evt.touch.x = state.touches[i].x;
                evt.touch.y = state.touches[i].y;
                forwardEvent(evt);
            }
        }
        for(s32 i = 0; i < previous_state.count; i++) {
            bool found = false;
            for (s32 y = 0; y < state.count; y++) {
                if (state.touches[i].finger_id == previous_state.touches[y].finger_id) {
                    found = true;
                }
            }
            if (!found) {
                Event evt;
                evt.type = Event::TouchEnded;
                evt.touch.finger = state.touches[i].finger_id;
                evt.touch.x = state.touches[i].x;
                evt.touch.y = state.touches[i].y;
                forwardEvent(evt);
            }
        }
        previous_state = state;
    }
}


} // namespace priv
} // namespace sf
