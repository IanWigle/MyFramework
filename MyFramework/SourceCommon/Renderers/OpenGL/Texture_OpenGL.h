//
// Copyright (c) 2019 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Texture_OpenGL_H__
#define __Texture_OpenGL_H__

#include "../../Textures/TextureDefinition.h"

class Texture_OpenGL : public TextureDefinition
{
    friend class FBO_OpenGL;

protected:
    GLuint m_TextureID;

public:
    Texture_OpenGL();
    Texture_OpenGL(GLuint textureID);
    virtual ~Texture_OpenGL();

    // Setters.
    void SetWidth(uint32 w);
    void SetHeight(uint32 h);

    // Getters.
    virtual bool IsFullyLoaded() override;
    GLuint GetTextureID();

    // Actions.
    virtual void GenerateTexture(unsigned char* pImageBuffer, uint32 width, uint32 height) override;
    virtual void GenerateErrorTexture() override;
    virtual void Invalidate(bool cleanGLAllocs) override;
};

#endif //__Texture_OpenGL_H__
