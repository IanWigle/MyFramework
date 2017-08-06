//
// Copyright (c) 2015-2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MaterialDefinition_H__
#define __MaterialDefinition_H__

class MaterialManager;

class ExposedUniformValue
{
public:
#if MYFW_USING_WX
    std::string m_Name;
    int m_ControlID;
#endif

    union
    {
        float m_Float;
        float m_Vec2[2];
        float m_Vec3[3];
        float m_Vec4[4];
        unsigned char m_ColorByte[4];
        TextureDefinition* m_pTexture;
    };

    void SetToInitialValue(ExposedUniformType type);
};

extern const char* MaterialBlendTypeStrings[MaterialBlendType_NumTypes];

class MaterialDefinition : public CPPListNode, public RefCount
{
    friend class MaterialManager;

public:
    static const int MAX_MATERIAL_NAME_LEN = 128;

protected:
    bool m_UnsavedChanges;

    char m_Name[MAX_MATERIAL_NAME_LEN]; // if [0] == 0, material won't save to disk.
    MyFileObject* m_pFile;

    ShaderGroup* m_pShaderGroup;
    ShaderGroup* m_pShaderGroupInstanced;
    TextureDefinition* m_pTextureColor;

    MaterialBlendType m_BlendType;

public:
    ExposedUniformValue m_UniformValues[MyFileObjectShader::MAX_EXPOSED_UNIFORMS];

public:
    ColorByte m_ColorAmbient;
    ColorByte m_ColorDiffuse;
    ColorByte m_ColorSpecular;
    float m_Shininess;

    Vector2 m_UVScale;
    Vector2 m_UVOffset;

public:
    bool m_FullyLoaded;

public:
    MaterialDefinition();
    MaterialDefinition(ShaderGroup* pShader);
    MaterialDefinition(ShaderGroup* pShader, ColorByte colordiffuse);
    void Init();

    virtual ~MaterialDefinition();

    MaterialDefinition& operator=(const MaterialDefinition& other);

    void ImportFromFile();
    void MoveAssociatedFilesToFrontOfFileList();

    const char* GetName() { return m_Name; }
    void SetName(const char* name);

    MyFileObject* GetFile() { return m_pFile; }
    const char* GetMaterialDescription();
    const char* GetMaterialShortDescription();

    // Callbacks for when shader file finishes loading.
    static void StaticOnFileFinishedLoading(void* pObjectPtr, MyFileObject* pFile) { ((MaterialDefinition*)pObjectPtr)->OnFileFinishedLoading( pFile ); }
    void OnFileFinishedLoading(MyFileObject* pFile);

    void InitializeExposedUniformValues(bool maintainexistingvalues);
    void ImportExposedUniformValues(cJSON* jMaterial);
    void ExportExposedUniformValues(cJSON* jMaterial);

    void SetShader(ShaderGroup* pShader);
    ShaderGroup* GetShader() const { return m_pShaderGroup; }

    void SetShaderInstanced(ShaderGroup* pShader);
    ShaderGroup* GetShaderInstanced() const { return m_pShaderGroupInstanced; }    

    void SetTextureColor(TextureDefinition* pTexture);
    TextureDefinition* GetTextureColor() const { return m_pTextureColor; }

    void SetColorAmbient(ColorByte color) { m_ColorAmbient = color; }
    ColorByte GetColorAmbient() { return m_ColorAmbient; }
    void SetColorDiffuse(ColorByte color) { m_ColorDiffuse = color; }
    ColorByte GetColorDiffuse() { return m_ColorDiffuse; }
    void SetColorSpecular(ColorByte color) { m_ColorSpecular = color; }
    ColorByte GetColorSpecular() { return m_ColorSpecular; }

    void SetBlendType(MaterialBlendType transparenttype) { m_BlendType = transparenttype; }
    MaterialBlendType GetBlendType() { return m_BlendType; }
    bool IsTransparent(BaseShader* pShader);
    bool IsTransparent();

public:
#if MYFW_USING_WX
    enum RightClickOptions
    {
        RightClick_ViewInWatchWindow = 1000,
    };

    int m_ControlID_Shader;
    int m_ControlID_ShaderInstanced;

    // Memory panel callbacks
    static void StaticOnLeftClick(void* pObjectPtr, wxTreeItemId id, unsigned int count) { ((MaterialDefinition*)pObjectPtr)->OnLeftClick( count ); }
    void OnLeftClick(unsigned int count);

    static void StaticOnRightClick(void* pObjectPtr, wxTreeItemId id) { ((MaterialDefinition*)pObjectPtr)->OnRightClick(); }
    void OnRightClick();
    void OnPopupClick(wxEvent &evt); // used as callback for wxEvtHandler, can't be virtual(will crash, haven't looked into it).

    static void StaticOnDrag(void* pObjectPtr) { ((MaterialDefinition*)pObjectPtr)->OnDrag(); }
    void OnDrag();

    static void StaticOnLabelEdit(void* pObjectPtr, wxTreeItemId id, wxString newlabel) { ((MaterialDefinition*)pObjectPtr)->OnLabelEdit( newlabel ); }
    void OnLabelEdit(wxString newlabel);

    // Watch panel callbacks.
    static void StaticOnDropShader(void* pObjectPtr, int controlid, wxCoord x, wxCoord y) { ((MaterialDefinition*)pObjectPtr)->OnDropShader(controlid, x, y); }
    void OnDropShader(int controlid, wxCoord x, wxCoord y);

    static void StaticOnDropTexture(void* pObjectPtr, int controlid, wxCoord x, wxCoord y) { ((MaterialDefinition*)pObjectPtr)->OnDropTexture(controlid, x, y); }
    void OnDropTexture(int controlid, wxCoord x, wxCoord y);

    void SaveMaterial(const char* relativepath);
    void AddToWatchPanel(bool clearwatchpanel, bool showbuiltinuniforms, bool showexposeduniforms);
#endif //MYFW_USING_WX
};

#endif //__MaterialDefinition_H__
