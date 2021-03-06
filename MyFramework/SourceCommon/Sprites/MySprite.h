//
// Copyright (c) 2012-2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MySprite_H__
#define __MySprite_H__

#include "../Meshes/MySubmesh.h"
#include "../Shaders/VertexFormats.h"

class BaseShader;
class BufferDefinition;
class BufferManager;
class MaterialManager;
class MySprite;
class ShaderGroup;
class TextureDefinition;

extern char g_SpriteVertexIndices[6];

class MySprite : public MySubmesh, public RefCount
{
protected:
    bool m_SpriteIsStatic;
    //MaterialDefinition* m_pMaterial;

public:
    // store up the current size and texcoords, if these change we need to rebuild sprite
    Vector2 m_SpriteSize;
    Vector2 m_SpriteUVStart;
    Vector2 m_SpriteUVEnd;
    unsigned char m_SpriteJustification;

    //BufferDefinition* m_pVertexBuffer;
    //BufferDefinition* m_pIndexBuffer;

    //MyMatrix m_Position;
    //MyMatrix* m_pParentMatrix;
    //ColorByte m_Tint; // doesn't work anymore... use materials.

public:
    MySprite();
    MySprite(MySprite* pSprite, const char* category, BufferManager* pBufferManager);
    virtual ~MySprite();

    virtual MaterialDefinition* GetMaterial() { return m_pMaterial; }
    //virtual void SetMaterial(MaterialDefinition* pMaterial);

    virtual void Create(BufferManager* pBufferManager, const char* category, float spritew, float spriteh, float startu, float endu, float startv, float endv, unsigned char justificationflags, bool staticverts = false, bool facepositivez = false);
    virtual void Create(BufferManager* pBufferManager, float spritew, float spriteh, float startu, float endu, float startv, float endv, unsigned char justificationflags, bool staticverts = false, bool facepositivez = false);
    virtual void CreateSubsection(BufferManager* pBufferManager, const char* category, float spritew, float spriteh, float startu, float endu, float startv, float endv, unsigned char justificationflags, float spx = 0, float epx = 1, float spy = 0, float epy = 1, bool staticverts = false, bool facepositivez = false);
    virtual void CreateInPlace(BufferManager* pBufferManager, const char* category, float x, float y, float spritew, float spriteh, float startu, float endu, float startv, float endv, unsigned char justificationflags, bool staticverts = false, bool facepositivez = false);
    //MyMatrix GetPosition();
    //void SetPosition(Vector3 pos, bool setindentity = true);
    //void SetPosition(float x, float y, float z, bool setindentity = true);
    //void SetPosition(MyMatrix* mat);
    //void SetZRotation(float rotation);
    //void SetZRotationWithPivot(float rotation, bool preserveposition = true, Vector3* localpivot = 0);
    //void SetRST(Vector3 rot, Vector3 scale, Vector3 pos, bool setindentity = true);
    //void SetTransform(MyMatrix& mat);
    //ColorByte* GetTintPointer() { return &m_Tint; }
    //void SetTint(ColorByte tintcolor);
    virtual void FlipX();

    virtual bool Setup(MyMatrix* pMatProj, MyMatrix* pMatView, MyMatrix* pMatWorld);
    virtual void DrawNoSetup();
    virtual void DeactivateShader();
    virtual void Draw(MyMatrix* pMatProj, MyMatrix* pMatView, MyMatrix* pMatWorld, ShaderGroup* pShaderOverride = 0, bool hideFromDrawList = false);
    virtual void Draw(MaterialDefinition* pMaterial, MyMesh* pMesh, MyMatrix* pMatProj, MyMatrix* pMatView, MyMatrix* pMatWorld, Vector3* campos, Vector3* camrot, MyLight** lightptrs, int numlights, MyMatrix* shadowlightVP, TextureDefinition* pShadowTex, TextureDefinition* pLightmapTex, ShaderGroup* pShaderOverride, bool hideFromDrawList);

    Vertex_Base* GetVerts(bool markDirty);

    TextureDefinition* GetTexture();
    //ColorByte GetTint() { return m_Tint; }
};

#endif //__MySprite_H__
