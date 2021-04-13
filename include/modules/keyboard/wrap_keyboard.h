#pragma once

#include "modules/keyboard/keyboard.h"
#include "common/luax.h"

namespace Wrap_Keyboard
{
    int SetTextInput(lua_State * L);

    int Register(lua_State * L);
}
