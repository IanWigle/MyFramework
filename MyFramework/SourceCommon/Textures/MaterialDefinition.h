//
// Copyright (c) 2015-2018 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MaterialDefinition_H__
#define __MaterialDefinition_H__

class MaterialManager;
class EditorMainFrame_ImGui;

class ExposedUniformValue
{
public:
#if MYFW_EDITOR
#if MYFW_USING_WX
    int m_ControlID;
#endif
    std::string m_Name;
    ExposedUniformType m_Type; // used when reloading shader, needed to release ref on texture.
#endif //MYFW_EDITOR

    ExposedUniformValue()
    {
#if MYFW_EDITOR
        m_Type = ExposedUniformType_NotSet;
#endif //MYFW_EDITOR
    }

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
#if MYFW_USING_IMGUI
    friend class EditorMainFrame_ImGui;
#endif

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
    MaterialBlendFactors GetShaderBlendFactorSrc(BaseShader* pShader);
    MaterialBlendFactors GetShaderBlendFactorSrc();
    GLenum GetShaderBlendFactorSrc_OpenGL(BaseShader* pShader);
    GLenum GetShaderBlendFactorSrc_OpenGL();
    MaterialBlendFactors GetShaderBlendFactorDest(BaseShader* pShader);
    MaterialBlendFactors GetShaderBlendFactorDest();
    GLenum GetShaderBlendFactorDest_OpenGL();
    GLenum GetShaderBlendFactorDest_OpenGL(BaseShader* pShader);

public:
#if MYFW_EDITOR
    enum RightClickOptions
    {
        RightClick_ViewInWatchWindow = 1000,
        RightClick_UnloadFile,
        RightClick_FindAllReferences,
    };

    void OnPopupClick(MaterialDefinition* pMaterial, int id);

#if MYFW_USING_WX
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
    static void StaticOnDropShader(void* pObjectPtr, int controlid, int x, int y) { ((MaterialDefinition*)pObjectPtr)->OnDropShader(controlid, x, y); }
    void OnDropShader(int controlid, int x, int y);

    static void StaticOnRightClickShader(void* pObjectPtr, int controlid) { ((MaterialDefinition*)pObjectPtr)->OnRightClickShader(controlid); }
    void OnRightClickShader(int controlid);

    static void StaticOnDropTexture(void* pObjectPtr, int controlid, int x, int y) { ((MaterialDefinition*)pObjectPtr)->OnDropTexture(controlid, x, y); }
    void OnDropTexture(int controlid, int x, int y);

    static void StaticOnRightClickTexture(void* pObjectPtr, int controlid) { ((MaterialDefinition*)pObjectPtr)->OnRightClickTexture(controlid); }
    void OnRightClickTexture(int controlid);

    void AddToWatchPanel(bool clearwatchpanel, bool showbuiltinuniforms, bool showexposeduniforms);
#endif //MYFW_USING_WX
    bool IsReferencingFile(MyFileObject* pFile);

    void SaveMaterial(const char* relativepath);
#endif //MYFW_EDITOR
};

#endif //__MaterialDefinition_H__
