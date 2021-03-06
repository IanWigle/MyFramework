//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "MyFrameworkPCH.h"

#include "MySprite.h"
#include "SpriteBatch.h"
#include "../Meshes/BufferManager.h"
#include "../Renderers/BaseClasses/Renderer_Base.h"
#include "../Renderers/BaseClasses/Shader_Base.h"
#include "../Textures/MaterialDefinition.h"
#include "../Textures/TextureDefinition.h"

SpriteBatch::SpriteBatch()
{
    m_pMaterial = 0;
    m_NumSprites = 0;

    m_SpritesAllocated = 0;
    m_pVertexBuffer = 0;
    m_pIndexBuffer = 0;
}

SpriteBatch::~SpriteBatch()
{
    SAFE_RELEASE( m_pVertexBuffer );
    SAFE_RELEASE( m_pIndexBuffer );

    SAFE_RELEASE( m_pMaterial );
}

void SpriteBatch::SetMaterial(MaterialDefinition* pMaterial)
{
    if( pMaterial )
        pMaterial->AddRef();
    SAFE_RELEASE( m_pMaterial );
    m_pMaterial = pMaterial;
}

void SpriteBatch::AllocateVertices(BufferManager* pBufferManager, int numsprites)
{
    MyAssert( m_pVertexBuffer == 0 );

    m_SpritesAllocated = numsprites;

    Vertex_Base* pVerts = (Vertex_Base*)MyNew Vertex_Sprite[numsprites*4];
    MyAssert( pVerts );

    GLushort* pIndices = MyNew GLushort[numsprites*6];
    MyAssert( pIndices );

    // allocate 2 empty buffers, will be filled by subbufferdata elsewhere.
    m_pVertexBuffer = pBufferManager->CreateBuffer( pVerts, sizeof(Vertex_Sprite)*numsprites*4, MyRE::BufferType_Vertex, MyRE::BufferUsage_DynamicDraw, false, 2, VertexFormat_Sprite, "SpriteBatch", "Verts" );

    for( GLushort i=0; i<numsprites; i++ )
    {
        pIndices[i*6 + 0] = i*4 + g_SpriteVertexIndices[0];
        pIndices[i*6 + 1] = i*4 + g_SpriteVertexIndices[1];
        pIndices[i*6 + 2] = i*4 + g_SpriteVertexIndices[2];
        pIndices[i*6 + 3] = i*4 + g_SpriteVertexIndices[3];
        pIndices[i*6 + 4] = i*4 + g_SpriteVertexIndices[4];
        pIndices[i*6 + 5] = i*4 + g_SpriteVertexIndices[5];
    }

    m_pIndexBuffer = pBufferManager->CreateBuffer( pIndices, sizeof(GLushort)*numsprites*6, MyRE::BufferType_Index, MyRE::BufferUsage_StaticDraw, true, 1, VertexFormat_None, "SpriteBatch", "Indices" );
}

void SpriteBatch::AddSprite(MyMatrix* pMatWorld, MySprite* pSprite)
{
    Vertex_Sprite* pSpriteVerts = (Vertex_Sprite*)pSprite->GetVerts( false );
    MyMatrix spriteTransform = *pMatWorld; //pSprite->GetPosition();
    Vertex_Sprite* pBatchVerts = (Vertex_Sprite*)m_pVertexBuffer->GetData( true );

    MyAssert( m_NumSprites < m_SpritesAllocated - 1 );
    if( m_NumSprites >= m_SpritesAllocated - 1 )
        return;

    for( int i=0; i<4; i++ )
    {
        //Vector2 pos( pSpriteVerts[0+i].x, pSpriteVerts[0+i].y );
        //pBatchVerts[m_NumSprites*4 + i] = spriteTransform * pos;

        pBatchVerts[m_NumSprites*4 + i] = pSpriteVerts[0+i];
        *((Vector2*)(&pBatchVerts[m_NumSprites*4 + i].x)) = spriteTransform * (*((Vector2*)(&pSpriteVerts[0+i].x)));
        
        //pBatchVerts[m_NumSprites*4 + i] = pSpriteVerts[0+i];
        //pBatchVerts[m_NumSprites*4 + i].x += spriteTransform.m41;
        //pBatchVerts[m_NumSprites*4 + i].y += spriteTransform.m42;
    }

    m_NumSprites++;
}

void SpriteBatch::Draw(MyMatrix* pMatProj, MyMatrix* pMatView)
{
    if( m_pMaterial == nullptr || m_pMaterial->GetTextureColor() == nullptr || m_pMaterial->GetTextureColor()->IsFullyLoaded() == false ||
        m_pMaterial->GetShader() == nullptr || m_NumSprites == 0 )
        return;

    MyMatrix pos;
    pos.SetIdentity();

    if( m_pVertexBuffer->IsDirty() )
    {
        m_pVertexBuffer->Rebuild( 0, sizeof(Vertex_Sprite)*m_NumSprites*4 );
    }
    if( m_pIndexBuffer->IsDirty() )
    {
        m_pIndexBuffer->Rebuild();
    }
    MyAssert( m_pVertexBuffer->IsDirty() == false && m_pIndexBuffer->IsDirty() == false );

    Shader_Base* pShader = (Shader_Base*)m_pMaterial->GetShader()->GlobalPass();
    if( pShader == 0 )
        return;

    // Enable blending if necessary. TODO: Sort draws and only set this once.
    if( m_pMaterial->IsTransparent( pShader ) )
    {
        g_pRenderer->SetBlendEnabled( true );
        g_pRenderer->SetBlendFunc( MyRE::BlendFactor_SrcAlpha, MyRE::BlendFactor_OneMinusSrcAlpha );
    }

    // Draw the contents of the buffers.
    if( pShader->ActivateAndProgramShader(
            m_pVertexBuffer, m_pIndexBuffer, MyRE::IndexType_U16,
            pMatProj, pMatView, &pos, m_pMaterial ) )
    {
#if USE_D3D
        g_pD3DContext->DrawIndexed( m_NumSprites*6, 0, 0 );
#else
        g_pRenderer->DrawElements( MyRE::PrimitiveType_Triangles, m_NumSprites*6, MyRE::IndexType_U16, 0, false );
#endif
        pShader->DeactivateShader( m_pVertexBuffer, true );
    }

    // Always disable blending.
    g_pRenderer->SetBlendEnabled( false );

    return;
}
