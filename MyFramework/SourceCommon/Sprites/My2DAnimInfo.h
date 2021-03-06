//
// Copyright (c) 2016-2019 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __My2DAnimInfo_H__
#define __My2DAnimInfo_H__

class MaterialDefinition;
class MaterialManager;
class MyFileObject;
class SpriteSheet;

//====================================================================================================
// My2DAnimationFrame
//====================================================================================================

class My2DAnimationFrame
{
    friend class My2DAnimation;
    friend class My2DAnimInfo;
    friend class EditorMainFrame_ImGui;

protected:
    MaterialDefinition* m_pMaterial;
    float m_Duration;

public:
    My2DAnimationFrame();
    ~My2DAnimationFrame();

    // Getters.
    MaterialDefinition* GetMaterial() { return m_pMaterial; }
    float GetDuration() { return m_Duration; }

    // Setters.
    void SetMaterial(MaterialDefinition* pMaterial);
    void SetDuration(float duration) { m_Duration = duration; }
};

//====================================================================================================
// My2DAnimation
//====================================================================================================

class My2DAnimation
{
    friend class My2DAnimInfo;
    friend class EditorMainFrame_ImGui;

protected:
    static const int MAX_ANIMATION_NAME_LEN = 32;

    char m_Name[MAX_ANIMATION_NAME_LEN+1];
    MyList<My2DAnimationFrame*> m_Frames;

public:
    // Getters.
    const char* GetName() { return m_Name; }
    uint32 GetFrameCount();
    My2DAnimationFrame* GetFrameByIndex(uint32 frameIndex);
    My2DAnimationFrame* GetFrameByIndexClamped(uint32 frameIndex);

    // Setters.
    void SetName(const char* name);
};

//====================================================================================================
// My2DAnimInfo
//====================================================================================================

class My2DAnimInfo : public RefCount
{
    friend class EditorMainFrame_ImGui;

public:
    static const unsigned int MAX_ANIMATIONS = 10; // TODO: Fix this hardcodedness.
    static const unsigned int MAX_FRAMES_IN_ANIMATION = 10; // TODO: Fix this hardcodedness.

protected:
    bool m_AnimationFileLoaded;
    MyFileObject* m_pSourceFile;
    MyList<My2DAnimation*> m_Animations;

public:
    My2DAnimInfo();
    virtual ~My2DAnimInfo();

    bool LoadAnimationControlFile(MaterialManager* pMaterialManager);

    // Getters.
    uint32 GetNumberOfAnimations();
    My2DAnimation* GetAnimationByName(const char* name);
    uint32 GetAnimationIndexByName(const char* name);
    My2DAnimation* GetAnimationByIndex(uint32 animIndex);
    My2DAnimation* GetAnimationByIndexClamped(uint32 animIndex);
    MyFileObject* GetSourceFile() { return m_pSourceFile; }

    // Setters.
    void SetSourceFile(MyFileObject* pSourceFile);

#if MYFW_EDITOR
    void LoadFromSpriteSheet(SpriteSheet* pSpriteSheet, float duration);
    void SaveAnimationControlFile();

    void OnAddAnimationPressed();
    void OnRemoveAnimationPressed(unsigned int animIndex);
    void OnRemoveFramePressed(unsigned int animIndex, unsigned int frameIndex);
    void OnAddFramePressed(int animIndex);
    void OnSaveAnimationsPressed();
#endif
};

#endif //__My2DAnimInfo_H__
