//
// Copyright (c) 2018 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Renderer_Base_H__
#define __Renderer_Base_H__

class Renderer_Base;
extern Renderer_Base* g_pRenderer;

class MyViewport
{
protected:
    uint32 m_X; // Bottom Left X,Y.
    uint32 m_Y;
    uint32 m_Width; // Width and Height of viewport.
    uint32 m_Height;

public:
    MyViewport() { Set( 0, 0, 0, 0 ); }
    MyViewport(uint32 x, uint32 y, uint32 w, uint32 h) { Set( x, y, w, h ); }

    // Getters.
    uint32 GetX() { return m_X; }
    uint32 GetY() { return m_Y; }
    uint32 GetWidth() { return m_Width; }
    uint32 GetHeight() { return m_Height; }

    // Setters.
    void Set(uint32 x, uint32 y, uint32 w, uint32 h) { m_X = x; m_Y = y; m_Width = w; m_Height = h; }
    void SetX(uint32 x) { m_X = x; }
    void SetY(uint32 y) { m_Y = y; }
    void SetWidth(uint32 w) { m_Width = w; }
    void SetHeight(uint32 h) { m_Height = h; }
};

class Renderer_Base
{
protected:
    bool m_IsValid;

    ColorFloat m_ClearColor;
    float m_ClearDepth;

public:
    Renderer_Base();
    virtual ~Renderer_Base();

    // Getters.
    bool IsValid() { return m_IsValid; }

    // Setters.

    // Events.
    virtual void OnSurfaceCreated();
    virtual void OnSurfaceChanged(uint32 x, uint32 y, uint32 width, uint32 height);
    virtual void OnSurfaceLost();

    // Actions.
    virtual void SetClearColor(ColorFloat color);
    virtual void SetClearDepth(float depth);
    virtual void ClearBuffers(bool clearColor, bool clearDepth, bool clearStencil) = 0;
    virtual void EnableViewport(MyViewport* pViewport, bool enableOrDisableScissorIfNeeded) = 0;

    virtual void DrawElements(MyRE::PrimitiveTypes mode, GLsizei count, MyRE::IndexTypes IBOType, const GLvoid* indices, bool hideFromDrawList) = 0;
    virtual void DrawArrays(MyRE::PrimitiveTypes mode, GLint first, GLsizei count, bool hideFromDrawList) = 0;
};

#endif //__Renderer_Base_H__
