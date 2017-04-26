//
// Copyright (c) 2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __SoundPlayerOpenSL_H__
#define __SoundPlayerOpenSL_H__

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "../SourceCommon/Sound/WaveLoader.h"

struct SoundObject : public CPPListNode
{
    MyFileObject* m_pFile;
    MyWaveDescriptor m_WaveDesc; // contains pointer to data in fileobject buffer

    SoundObject();
    cJSON* ExportAsJSONObject();
};

class SoundChannel
{
public:
    SLObjectItf m_ppAudioPlayer; // using a buffer queue as input
};

class SoundPlayer
{
protected:
    static const int MAX_SOUNDS = 255;
    static const int MAX_QUEUED_SOUNDS = 10;
    static const int MAX_CHANNELS = 10;

    // OpenSL Objects
    SLObjectItf m_ppOpenSLEngine;
    SLObjectItf m_ppOutputMix;

    SoundChannel m_Channels[MAX_CHANNELS];

    SoundObject m_Sounds[MAX_SOUNDS];
    int m_QueuedSounds[MAX_QUEUED_SOUNDS];
    int m_NumQueuedSounds;

    void ReallyPlaySound(int soundid);

public:
    SoundPlayer();
    ~SoundPlayer();

    void OnFocusGained();
    void OnFocusLost();

    void Tick(double TimePassed);

    SoundObject* LoadSound(const char* fullpath);
    SoundObject* LoadSound(MyFileObject* pFile);

    void Shutdown();
    int PlaySound(SoundObject* pSoundObject);
    void StopSound(int soundid);
    void PauseSound(int soundid);
    void ResumeSound(int soundid);
    void PauseAll();
    void ResumeAll();

    void TestOpenSL_BufferQueue();
    void TestOpenSL_URILocator();
};

#endif //__SoundPlayerOpenSL_H__
