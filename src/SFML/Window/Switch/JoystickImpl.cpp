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
#include <switch.h>
#include <SFML/Window/JoystickImpl.hpp>


static HidNpadButton KEYS_BY_INDEX[] = {
    HidNpadButton_A,
    HidNpadButton_B,
    HidNpadButton_X,
    HidNpadButton_Y,
    HidNpadButton_StickL,
    HidNpadButton_StickR,
    HidNpadButton_L,
    HidNpadButton_R,
    HidNpadButton_ZL,
    HidNpadButton_ZR,
    HidNpadButton_Plus,
    HidNpadButton_Minus,
    HidNpadButton_Left,
    HidNpadButton_Up,
    HidNpadButton_Right,
    HidNpadButton_Down,
    HidNpadButton_StickLLeft,
    HidNpadButton_StickLUp,
    HidNpadButton_StickLRight,
    HidNpadButton_StickLDown,
    HidNpadButton_StickRLeft,
    HidNpadButton_StickRUp,
    HidNpadButton_StickRRight,
    HidNpadButton_StickRDown,
    HidNpadButton_LeftSL,
    HidNpadButton_LeftSR,
    HidNpadButton_RightSL,
    HidNpadButton_RightSR,
};

enum {
    JOYSTICK_LEFT = 0,
    JOYSTICK_RIGHT = 1,
};

#define NUM_KEYS_BY_INDEX ((int) (sizeof(KEYS_BY_INDEX) / sizeof(HidNpadButton)))


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
}



////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    PadState state;
    padInitialize(&state, (HidNpadIdType)index);
    return padIsConnected(&state);
}


////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    if (!isConnected(index))
        return (false);
    m_padId = (HidNpadIdType)index;
    padInitialize(&m_state, (HidNpadIdType)index);
    return true;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{

}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    // To implement
    JoystickCaps caps = JoystickCaps();
    caps.buttonCount = NUM_KEYS_BY_INDEX;
    caps.axes[Joystick::X] = true;
    caps.axes[Joystick::Y] = true;
    return caps;
}


////////////////////////////////////////////////////////////
Joystick::Identification JoystickImpl::getIdentification() const
{
    Joystick::Identification ident = Joystick::Identification();
    ident.name = sf::String("Generic Switch controller");
    ident.productId = ident.vendorId = 0;
    return ident;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update()
{
    padUpdate(&m_state);
    auto sfmlState = JoystickState();
    u64 keys = padGetButtonsDown(&m_state);

    sfmlState.connected = isConnected(m_padId);
    for (int i = 0; i < NUM_KEYS_BY_INDEX; i++)
        sfmlState.buttons[i] = (keys & KEYS_BY_INDEX[i]) != 0;

    HidAnalogStickState left = padGetStickPos(&m_state, JOYSTICK_LEFT);
    HidAnalogStickState right = padGetStickPos(&m_state, JOYSTICK_RIGHT);
    sfmlState.axes[Joystick::X] = left.x;
    sfmlState.axes[Joystick::Y] = left.y;
    sfmlState.axes[Joystick::U] = right.x;
    sfmlState.axes[Joystick::V] = right.y;
    // To implement
    return sfmlState;
}

} // namespace priv

} // namespace sf
