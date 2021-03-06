//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "MyFrameworkPCH.h"

#include "VertexFormats.h"

unsigned int g_VertexFormatSizes[VertexFormat_NumFormats] = // ADDING_NEW_VertexFormat
{
    0,
    sizeof( Vertex_Sprite ),
    sizeof( Vertex_XYZ ),
    sizeof( Vertex_XYZUV ),
    sizeof( Vertex_XYZUV_RGBA ),
    sizeof( Vertex_XYZUVNorm ),
    sizeof( Vertex_XYZUVNorm_1Bones ),
    sizeof( Vertex_XYZUVNorm_2Bones ),
    sizeof( Vertex_XYZUVNorm_3Bones ),
    sizeof( Vertex_XYZUVNorm_4Bones ),
    sizeof( Vertex_XYZNorm ),
    sizeof( Vertex_PointSprite ),
    sizeof( Vertex_XYZUVNorm_RGBA ),
    sizeof( Vertex_XYZUVNorm_RGBA_1Bones ),
    sizeof( Vertex_XYZUVNorm_RGBA_2Bones ),
    sizeof( Vertex_XYZUVNorm_RGBA_3Bones ),
    sizeof( Vertex_XYZUVNorm_RGBA_4Bones ),
};
