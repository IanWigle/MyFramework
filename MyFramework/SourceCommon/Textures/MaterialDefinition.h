//
// Copyright (c) 2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MaterialDefinition_H__
#define __MaterialDefinition_H__

class MaterialManager;

extern const char* MaterialBlendTypeStrings[MaterialBlendType_NumTypes];

class MaterialDefinition : public CPPListNode, public RefCount
{
    friend class MaterialManager;

public:
    static const int MAX_MATERIAL_NAME_LEN = 32;

public: // for now.
    bool m_UnsavedChanges;

    char m_Name[MAX_MATERIAL_NAME_LEN]; // if [0] == 0, material won't save to disk.
    MyFileObject* m_pFile;

protected:
    ShaderGroup* m_pShaderGroup;
    TextureDefinition* m_pTextureColor;

    MaterialBlendType m_BlendType;

public:
    ColorByte m_ColorAmbient;
    ColorByte m_ColorDiffuse;
    ColorByte m_ColorSpecular;
    float m_Shininess;

public:
    bool m_FullyLoaded;

public:
    MaterialDefinition();
    MaterialDefinition(ShaderGroup* pShader);
    MaterialDefinition(ShaderGroup* pShader, ColorByte colordiffuse);
    void Init();

    virtual ~MaterialDefinition();

    void ImportFromFile();

    const char* GetName() { return m_Name; }
    void SetName(const char* name);

    void SetShader(ShaderGroup* pShader);
    ShaderGroup* GetShader() { return m_pShaderGroup; }

    void SetTextureColor(TextureDefinition* pTexture);
    TextureDefinition* GetTextureColor() { return m_pTextureColor; }

    void SetColorAmbient(ColorByte color) { m_ColorAmbient = color; }
    ColorByte GetColorAmbient() { return m_ColorAmbient; }
    void SetColorDiffuse(ColorByte color) { m_ColorDiffuse = color; }
    ColorByte GetColorDiffuse() { return m_ColorDiffuse; }
    void SetColorSpecular(ColorByte color) { m_ColorSpecular = color; }
    ColorByte GetColorSpecular() { return m_ColorSpecular; }

    void SetBlendType(MaterialBlendType transparenttype) { m_BlendType = transparenttype; }
    MaterialBlendType GetBlendType() { return m_BlendType; }
    bool IsTransparent(Shader_Base* pShader);

public:
#if MYFW_USING_WX
    enum RightClickOptions
    {
        RightClick_ViewInWatchWindow = 1000,
    };

    static void StaticOnLeftClick(void* pObjectPtr, unsigned int count) { ((MaterialDefinition*)pObjectPtr)->OnLeftClick( count ); }
    void OnLeftClick(unsigned int count);
    static void StaticOnRightClick(void* pObjectPtr, wxTreeItemId id) { ((MaterialDefinition*)pObjectPtr)->OnRightClick(); }
    void OnRightClick();
    void OnPopupClick(wxEvent &evt); // used as callback for wxEvtHandler, can't be virtual(will crash, haven't looked into it).
    static void StaticOnDrag(void* pObjectPtr) { ((MaterialDefinition*)pObjectPtr)->OnDrag(); }
    void OnDrag();

    static void StaticOnDropShader(void* pObjectPtr, int controlid, wxCoord x, wxCoord y) { ((MaterialDefinition*)pObjectPtr)->OnDropShader(controlid, x, y); }
    void OnDropShader(int controlid, wxCoord x, wxCoord y);
    static void StaticOnDropTexture(void* pObjectPtr, int controlid, wxCoord x, wxCoord y) { ((MaterialDefinition*)pObjectPtr)->OnDropTexture(controlid, x, y); }
    void OnDropTexture(int controlid, wxCoord x, wxCoord y);

    static void StaticOnLabelEdit(void* pObjectPtr, wxString newlabel) { ((MaterialDefinition*)pObjectPtr)->OnLabelEdit( newlabel ); }
    void OnLabelEdit(wxString newlabel);

    void SaveMaterial();
#endif //MYFW_USING_WX
};

#endif //__MaterialDefinition_H__
