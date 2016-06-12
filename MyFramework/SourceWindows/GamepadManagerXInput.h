//
// Copyright (c) 2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __GamepadManagerXInput_H__
#define __GamepadManagerXInput_H__

class GamepadManagerXInput : public GamepadManager
{
protected:
    void NormalizeStick(Vector2 stick, float deadzone, Vector2* stickout);
    void NormalizeTrigger(float trigger, float deadzone, float* triggerout);

public:
    GamepadManagerXInput();
    virtual ~GamepadManagerXInput();

    virtual void Initialize();
    virtual void Tick(double TimePassed);
};

#endif //__GamepadManagerXInput_H__