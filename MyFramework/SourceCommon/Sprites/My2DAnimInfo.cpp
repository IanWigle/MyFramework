//
// Copyright (c) 2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CommonHeader.h"

My2DAnimInfo::My2DAnimInfo()
{
    m_pSourceFile = 0;
}

My2DAnimInfo::~My2DAnimInfo()
{
    for( unsigned int i=0; i<m_pAnimations.Length(); i++ )
    {
        for( unsigned int j=0; j<m_pAnimations[i]->m_Frames.Length(); j++ )
        {
            delete m_pAnimations[i]->m_Frames[j];
        }

        delete m_pAnimations[i];
    }
    SAFE_RELEASE( m_pSourceFile );
}

#if MYFW_USING_WX
void My2DAnimInfo::SaveAnimationControlFile()
{
    char filename[MAX_PATH];
    m_pSourceFile->GenerateNewFullPathExtensionWithSameNameInSameFolder( ".my2daniminfo", filename, MAX_PATH );

    cJSON* jRoot = cJSON_CreateObject();

    cJSON* jAnimArray = cJSON_CreateArray();
    cJSON_AddItemToObject( jRoot, "Anims", jAnimArray );

    for( unsigned int i=0; i<m_pAnimations.Count(); i++ )
    {
        cJSON* jAnim = cJSON_CreateObject();

        cJSON_AddItemToArray( jAnimArray, jAnim );

        cJSON_AddStringToObject( jAnim, "Name", m_pAnimations[i]->m_Name );

        // TODO: write out frame info
        MyAssert( false );
    }

    // dump animarray to disk
    char* jsonstr = cJSON_Print( jRoot );
    cJSON_Delete( jRoot );

    FILE* pFile;
#if MYFW_WINDOWS
    fopen_s( &pFile, filename, "wb" );
#else
    pFile = fopen( filename, "wb" );
#endif
    fprintf( pFile, "%s", jsonstr );
    fclose( pFile );

    cJSONExt_free( jsonstr );
}
#endif

void My2DAnimInfo::LoadAnimationControlFile(char* buffer)
{
    MyAssert( buffer != 0 );

    // if the file doesn't exist, do nothing for now.
    if( buffer == 0 )
    {
        MyAssert( m_pAnimations.Count() == 0 );
    }
    else
    {
        cJSON* jRoot = cJSON_Parse( buffer );

        if( jRoot )
        {
            cJSON* jAnimArray = cJSON_GetObjectItem( jRoot, "Anims" );

            int numanims = cJSON_GetArraySize( jAnimArray );
            m_pAnimations.AllocateObjects( numanims );
            for( int i=0; i<numanims; i++ )
            {
                cJSON* jAnim = cJSON_GetArrayItem( jAnimArray, i );

                My2DAnimation* pAnim = MyNew My2DAnimation;
                m_pAnimations.Add( pAnim );

                cJSON* jName = cJSON_GetObjectItem( jAnim, "Name" );
                if( jName )
                    pAnim->SetName( jName->valuestring );

                cJSON* jFrameArray = cJSON_GetObjectItem( jRoot, "Anims" );

                int numframes = cJSON_GetArraySize( jFrameArray );
                pAnim->m_Frames.AllocateObjects( numframes );
                for( int i=0; i<numframes; i++ )
                {
                    cJSON* jFrame = cJSON_GetArrayItem( jFrameArray, i );

                    My2DAnimationFrame* pFrame = MyNew My2DAnimationFrame();
                    pAnim->m_Frames.Add( pFrame );

                    cJSON* jMatName = cJSON_GetObjectItem( jAnim, "Material" );
                    if( jMatName )
                        pFrame->SetMaterialName( jMatName->valuestring );

                    cJSONExt_GetFloat( jFrame, "Duration", &pFrame->m_Duration );
                    cJSONExt_GetFloatArray( jFrame, "UVScale", &pFrame->m_UVScale.x, 2 );
                    cJSONExt_GetFloatArray( jFrame, "UVOffset", &pFrame->m_UVOffset.x, 2 );
                }
            }
        }

        cJSON_Delete( jRoot );
    }
}
