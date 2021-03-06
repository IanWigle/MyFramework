//
// Copyright (c) 2016-2018 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "MyFrameworkPCH.h"

#include "RenderGraph_Base.h"
#include "../Core/GameCore.h"
#include "../DataTypes/MyActivePool.h"
#include "../Renderers/BaseClasses/Renderer_Enums.h"
#include "../Textures/MaterialDefinition.h"

void RenderGraphObject::Clear()
{
    m_Flags = RenderGraphFlag_Opaque;
    m_pMaterial = nullptr;
    m_WaitingForMaterialToFinishLoading = false;

#if MYFW_EDITOR
    m_EditorObject = false;
#endif

    m_Layers = 0;
    m_pTransform = nullptr;
    m_pMesh = nullptr;
    m_pSubmesh = nullptr;
    m_Visible = false;

    m_GLPrimitiveType = MyRE::PrimitiveType_Undefined;
    m_PointSize = 0;

    m_pUserData = nullptr;
}

void RenderGraphObject::SetMaterial(MaterialDefinition* pNewMaterial, bool updateTransparencyFlags)
{
    m_pMaterial = pNewMaterial;

    if( pNewMaterial && pNewMaterial->IsShaderLoaded() == false )
    {
        m_WaitingForMaterialToFinishLoading = true;
        return;
    }

    m_WaitingForMaterialToFinishLoading = false;

    if( updateTransparencyFlags )
    {
        RenderGraphFlags flags = (RenderGraphFlags)(m_Flags & ~(RenderGraphFlag_Opaque | RenderGraphFlag_Transparent | RenderGraphFlag_Emissive));
        if( pNewMaterial )
        {
            if( pNewMaterial->IsTransparent() )
                flags = (RenderGraphFlags)(flags | RenderGraphFlag_Transparent);
            else
                flags = (RenderGraphFlags)(flags | RenderGraphFlag_Opaque);

            if( pNewMaterial->IsEmissive() )
                flags = (RenderGraphFlags)(flags | RenderGraphFlag_Emissive);
        }
        else
        {
            flags = (RenderGraphFlags)(flags | RenderGraphFlag_Opaque);
        }

        m_Flags = flags;
    }
}

RenderGraph_Base::RenderGraph_Base(GameCore* pGameCore)
{
    m_pGameCore = pGameCore;

    m_pObjectPool.AllocateObjects( 100000 );
}

RenderGraph_Base::~RenderGraph_Base()
{
}

RenderGraphObject* RenderGraph_Base::AddObject(MyMatrix* pTransform, MyMesh* pMesh, MySubmesh* pSubmesh, MaterialDefinition* pMaterial, MyRE::PrimitiveTypes primitiveType, int pointSize, unsigned int layers, void* pUserData)
{
    // Add the object with the opaque flag set.
    RenderGraphObject* pRenderGraphObject = AddObjectWithFlagOverride( pTransform, pMesh, pSubmesh, pMaterial, primitiveType, pointSize, RenderGraphFlag_Opaque, layers, pUserData);

    // Set the material again, this time also overwrite the opacity flags with the material setting.
    pRenderGraphObject->SetMaterial( pMaterial, true );

    return pRenderGraphObject;
}

bool RenderGraph_Base::ShouldObjectBeDrawn(RenderGraphObject* pObject, bool drawOpaques, EmissiveDrawOptions emissiveDrawOption, unsigned int layersToRender)
{
    // Forward Passes:
    //   opaque and opaque/emissive
    //   transparent and transparent/emissive

    // Deferred Passes:
    //   deferred -> opaque
    //   forward -> opaque/emissive
    //   forward -> transparent and transparent/emissive

    // If we're waiting for the material to finish loading, check if it's finished loading.
    if( pObject->IsWaitingForMaterialToFinishLoading() )
    {
        // Check if material finished loading.
        pObject->SetMaterial( pObject->GetMaterial(), true );
        
        // Check again; if we're still waiting for the material to finish loading, don't draw this object.
        if( pObject->IsWaitingForMaterialToFinishLoading() )
            return false;
    }

    RenderGraphFlags objectFlags = pObject->GetFlags();

    bool isOpaque      = objectFlags & RenderGraphFlag_Opaque   ? true : false;
    bool isEmissive    = objectFlags & RenderGraphFlag_Emissive ? true : false;

    // If we're drawing opaques and this object is transparent, kick out.
    if( drawOpaques & !isOpaque )
        return false;

    // If we're drawing transparents and this object is opaque, kick out.
    if( !drawOpaques & isOpaque )
        return false;

    // If we don't want to draw emissive objects and this object is emissive, kick out.
    if( emissiveDrawOption == EmissiveDrawOption_NoEmissives && isEmissive )
        return false;

    // If we exclusively want to draw emissive objects and this object is not emissive, kick out.
    if( emissiveDrawOption == EmissiveDrawOption_OnlyEmissives && isEmissive == false )
        return false;

    if( (pObject->m_Layers & layersToRender) == 0 )
        return false;
        
    MyAssert( pObject->m_pSubmesh );

    if( pObject->m_pSubmesh == nullptr ) //|| pObject->GetMaterial() == nullptr )
        return false;

    if( pObject->m_Visible == false )
        return false;

    return true;
}