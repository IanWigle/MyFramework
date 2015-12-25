//
// Copyright (c) 2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Box2DWorld_H__
#define __Box2DWorld_H__

class Box2DWorld;
class Box2DDebugDraw;
class MaterialDefinition;

extern Box2DWorld* g_pBox2DWorld;

class Box2DWorld
{
public:
    b2World* m_pWorld;
    Box2DDebugDraw* m_pDebugDraw;

public:
    Box2DWorld(MaterialDefinition* debugdrawmaterial, MyMatrix* matviewproj);
    ~Box2DWorld();

    void CreateWorld(MaterialDefinition* debugdrawmaterial, MyMatrix* matviewproj);
    void PhysicsStep();
    void Cleanup();
};

#endif //__Box2DWorld_H__
