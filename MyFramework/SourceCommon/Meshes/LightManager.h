//
// Copyright (c) 2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __LightManager_H__
#define __LightManager_H__

class LightManager;

extern LightManager* g_pLightManager;

class LightManager
{
    static const int MAX_LIGHTS = 8;

public:
    CPPListHead m_LightList;
    MyLight m_LightArray[MAX_LIGHTS];

public:
    LightManager();
    virtual ~LightManager();

    MyLight* CreateLight();
    void DestroyLight(MyLight* pLight);

    int FindNearestLights(int numtofind, Vector3 pos, MyLight** ppLightArray);
};

#endif //__LightManager_H__