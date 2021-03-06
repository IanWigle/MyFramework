//
// Copyright (c) 2018-2019 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __Renderer_Enums_H__
#define __Renderer_Enums_H__

namespace MyRE // MyRendererEnums
{
    enum PrimitiveTypes
    {
        PrimitiveType_Points,
        PrimitiveType_Lines,
        PrimitiveType_LineLoop,
        PrimitiveType_LineStrip,
        PrimitiveType_Triangles,
        PrimitiveType_TriangleStrip,
        PrimitiveType_TriangleFan,
        PrimitiveType_Undefined,
    };

    enum FrontFaceWindings
    {
        FrontFaceWinding_Clockwise,
        FrontFaceWinding_CounterClockwise,
        FrontFaceWinding_NumTypes,
    };

    enum CullModes
    {
        CullMode_Front,
        CullMode_Back,
        CullMode_FrontAndBack,
        CullMode_NumModes,
    };

    enum AttributeTypes
    {
        AttributeType_Byte,
        AttributeType_UByte,
        AttributeType_Short,
        AttributeType_UShort,
        AttributeType_Int,
        AttributeType_UInt,
        AttributeType_HalfFloat,
        AttributeType_Float,
        AttributeType_Double,
        AttributeType_Fixed,
        AttributeType_Int_2_10_10_10_Rev,
        AttributeType_Unsigned_Int_2_10_10_10_Rev,
        AttributeType_Unsigned_Int_10f_11f_11f_Rev,
        AttributeType_NumTypes,
    };

    enum BufferTypes
    {
        BufferType_Vertex,
        BufferType_Index,
        BufferType_NumTypes,
    };

    enum BufferUsages
    {
        BufferUsage_StreamDraw,
        BufferUsage_StaticDraw,
        BufferUsage_DynamicDraw,
        BufferUsage_NumTypes,
    };

    enum IndexTypes
    {
        IndexType_U8,
        IndexType_U16,
        IndexType_U32,
        IndexType_NumTypes,
        IndexType_Undefined = IndexType_NumTypes,
    };

    enum MaterialBlendTypes
    {
        MaterialBlendType_UseShaderValue,
        MaterialBlendType_Off,
        MaterialBlendType_On,
        MaterialBlendType_NumTypes,
        MaterialBlendType_NotSet = MaterialBlendType_NumTypes + 20,
    };

    enum BlendFactors
    {
        BlendFactor_Zero,
        BlendFactor_One,
        BlendFactor_SrcColor,
        BlendFactor_OneMinusSrcColor,
        BlendFactor_DstColor,
        BlendFactor_OneMinusDstColor,
        BlendFactor_SrcAlpha,
        BlendFactor_OneMinusSrcAlpha,
        BlendFactor_DstAlpha,
        BlendFactor_OneMinusDstAlpha,
        //BlendFactor_ConstantColor,
        //BlendFactor_OneMinusConstantColor,
        //BlendFactor_ConstantAlpha,
        //BlendFactor_OneMinusConstantAlpha,
        BlendFactor_SrcAlphaSaturate,
        //BlendFactor_Src1Color,
        //BlendFactor_OneMinusSrc1Color,
        //BlendFactor_Src1Alpha,
        //BlendFactor_OneMinusSrc1Alpha,
        BlendFactor_NumTypes,
    };

    enum MinFilters
    {
        MinFilter_Nearest,
        MinFilter_Linear,
        MinFilter_Nearest_MipmapNearest,
        MinFilter_Linear_MipmapNearest,
        MinFilter_Nearest_MipmapLinear,
        MinFilter_Linear_MipmapLinear,
        MinFilter_NumTypes,
    };

    enum MagFilters
    {
        MagFilter_Nearest,
        MagFilter_Linear,
        MagFilter_NumTypes,
    };

    enum WrapModes
    {
        WrapMode_Clamp,
        WrapMode_Repeat,
        WrapMode_MirroredRepeat,
        WrapMode_NumTypes,
    };

    enum PixelFormats
    {
        PixelFormat_StencilIndex,
        PixelFormat_DepthComponent,
        PixelFormat_DepthStencil,
        PixelFormat_Red,
        PixelFormat_Green,
        PixelFormat_Blue,
        PixelFormat_RGB,
        PixelFormat_BGR,
        PixelFormat_RGBA,
        PixelFormat_BGRA,
        PixelFormat_NumTypes,
    };

    enum PixelDataTypes
    {
        PixelDataType_UByte,
        PixelDataType_Byte,
        PixelDataType_UShort,
        PixelDataType_Short,
        PixelDataType_UInt,
        PixelDataType_Int,
        PixelDataType_HalfFloat,
        PixelDataType_Float,
        PixelDataType_UByte_3_3_2,
        PixelDataType_UByte_2_3_3_Rev,
        PixelDataType_UShort_5_6_5,
        PixelDataType_UShort_5_6_5_Rev,
        PixelDataType_UShort_4_4_4_4,
        PixelDataType_UShort_4_4_4_4_Rev,
        PixelDataType_UShort_5_5_5_1,
        PixelDataType_UShort_1_5_5_5_Rev,
        PixelDataType_UInt_8_8_8_8,
        PixelDataType_UInt_8_8_8_8_Rev,
        PixelDataType_UInt_10_10_10_2,
        PixelDataType_UInt_2_10_10_10_Rev,
        PixelDataType_UInt_24_8,
        PixelDataType_UInt_10f_11f_11f_Rev,
        PixelDataType_UInt_5_9_9_9_Rev,
        PixelDataType_Float_32_UInt_24_8_Rev,
        PixelDataType_NumTypes,
    };

    enum DepthFuncs
    {
        DepthFunc_Never,
        DepthFunc_Less,
        DepthFunc_Equal,
        DepthFunc_LEqual,
        DepthFunc_Greater,
        DepthFunc_NotEqual,
        DepthFunc_GEqual,
        DepthFunc_Always,
        DepthFunc_NumTypes,
    };

    enum PolygonDrawModes
    {
        PolygonDrawMode_Point,
        PolygonDrawMode_Line,
        PolygonDrawMode_Fill,
        PolygonDrawMode_NumModes,
    };

    extern const char* MaterialBlendTypeStrings[MyRE::MaterialBlendType_NumTypes];
    extern const int IndexTypeByteSizes[MyRE::IndexType_NumTypes];
} //namespace MyRE

#endif //__Renderer_Enums_H__
