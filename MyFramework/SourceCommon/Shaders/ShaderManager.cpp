//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CommonHeader.h"

ShaderManager* g_pShaderManager = 0;

BaseShader::BaseShader()
{
    Init_BaseShader();
    Init( ShaderPass_Main );
}

BaseShader::BaseShader(ShaderPassTypes type)
{
    Init_BaseShader();
    Init( type );
}

void BaseShader::Init_BaseShader()
{
    m_Initialized = false;
    m_ShaderFailedToCompile = false;

    m_pFilename = 0;
    m_pFile = 0;
    m_pFilePixelShader = 0;

    m_pVSPredefinitions = 0;
    m_pFSPredefinitions = 0;

    m_ProgramHandle = 0;
    m_VertexShaderHandle = 0;
    m_FragmentShaderHandle = 0;

    g_pShaderManager->AddShader( this );
}

BaseShader::~BaseShader()
{
    this->Remove();
    if( m_pFile )
        g_pFileManager->FreeFile( m_pFile );
    if( m_pFilePixelShader )
        g_pFileManager->FreeFile( m_pFilePixelShader );

    SAFE_DELETE_ARRAY( m_pVSPredefinitions );
    SAFE_DELETE_ARRAY( m_pFSPredefinitions );

    Invalidate( true );
}

void BaseShader::Init(ShaderPassTypes type)
{
    m_PassType = type;
}

void BaseShader::OverridePredefs(const char* VSpredef, const char* FSpredef, bool alsousedefaults)
{
    if( VSpredef )
    {
        int vslen = (int)strlen( VSpredef );
        if( alsousedefaults )
            vslen += strlen( VERTEXPREDEFINES );
        vslen += 1;
        char* newvsstr = MyNew char[vslen];
        strcpy_s( newvsstr, vslen, VSpredef );
        if( alsousedefaults )
            strcat_s( newvsstr, vslen, VERTEXPREDEFINES );
        m_pVSPredefinitions = newvsstr;
    }

    if( FSpredef )
    {
        int fslen = (int)strlen( FSpredef );
        if( alsousedefaults )
            fslen += strlen( FRAGMENTPREDEFINES );
        fslen += 1;
        char* newfsstr = MyNew char[fslen];
        strcpy_s( newfsstr, fslen, FSpredef );
        if( alsousedefaults )
            strcat_s( newfsstr, fslen, FRAGMENTPREDEFINES );
        m_pFSPredefinitions = newfsstr;
    }
}

void BaseShader::Invalidate(bool cleanglallocs)
{
    if( cleanglallocs )
    {
        CleanGLAllocations();
    }
    m_Initialized = false;

    m_ProgramHandle = 0;
    m_VertexShaderHandle = 0;
    m_FragmentShaderHandle = 0;

    m_ShaderFailedToCompile = false;
}

void BaseShader::CleanGLAllocations()
{
    if( m_Initialized )
    {
        m_Initialized = false;

        glDetachShader( m_ProgramHandle, m_VertexShaderHandle );
        glDetachShader( m_ProgramHandle, m_FragmentShaderHandle );    
        glDeleteShader( m_VertexShaderHandle );
        glDeleteShader( m_FragmentShaderHandle );
        glDeleteProgram( m_ProgramHandle );

        m_ProgramHandle = 0;
        m_VertexShaderHandle = 0;
        m_FragmentShaderHandle = 0;
    }

    m_ShaderFailedToCompile = false;
}

void BaseShader::LoadFromFile(const char* filename)
{
    assert( filename != 0 );
    if( filename == 0 )
        return;

    m_pFilename = filename;
    LoadFromFile();
}

void BaseShader::LoadFromFile()
{
    assert( m_pFile == 0 );
#if MYFW_WP8
    char tempfilename[MAX_PATH];
    sprintf_s( tempfilename, MAX_PATH, "%s.vertex.cso", m_pFilename );
    m_pFile = RequestFile( tempfilename );
    sprintf_s( tempfilename, MAX_PATH, "%s.pixel.cso", m_pFilename );
    m_pFilePixelShader = RequestFile( tempfilename );
#else
    char tempfilename[MAX_PATH];
    sprintf_s( tempfilename, MAX_PATH, "%s.glsl", m_pFilename );
    MyFileObject* pFile = RequestFile( tempfilename );
    m_pFile = dynamic_cast<MyFileObjectShader*>( pFile );
    assert( m_pFile != 0 );
    if( m_pFile == 0 )
        g_pFileManager->FreeFile( pFile );
#endif
}

bool BaseShader::LoadAndCompile()
{
    assert( m_pFilePixelShader == 0 ); // TODO: see below, need to fix support for sep. vert/frag files.

    // if we already failed to compile, don't try again.
    if( m_ShaderFailedToCompile )
    {
        //LOGError( LOGTag, "BaseShader::LoadAndCompile - m_ShaderFailedToCompile\n" );
        return false;
    }

    if( m_pFile == 0 )
    {
        LoadFromFile();
        return false;
    }

    if( m_pFile->m_LoadFailed )
    {
        LOGInfo( LOGTag, "Shader failed to load\n" );
        return false;
    }

    // if the file isn't loaded, come back next frame and check again.
    if( m_pFile->m_FileReady == false || (m_pFilePixelShader != 0 && m_pFilePixelShader->m_FileReady == false) )
    {
        return false;
    }

    // scan file for #includes and add them to the list
    if( m_pFile->m_ScannedForIncludes == false )
        m_pFile->CheckFileForIncludesAndAddToList();

    // are all #includes loaded? if not drop out and check again next frame.
    bool loadcomplete = m_pFile->AreAllIncludesLoaded();
    if( loadcomplete == false )
        return false;

    // check how many shader chunks exist between this file and all others included.
    int numchunks = m_pFile->GetShaderChunkCount();

    // allocate buffers for all chunks + 2 more for the shader pass defines and the prevertex/prefrag strings
    numchunks += 2;
    const char** pStrings = MyNew const char*[numchunks];
    int* pLengths = MyNew int[numchunks];

    m_pFile->GetShaderChunks( &pStrings[2], &pLengths[2] );

    // stick our shader pass defines in the first slot.
    pStrings[0] = g_ShaderPassDefines[m_PassType];
    pLengths[0] = (int)strlen( g_ShaderPassDefines[m_PassType] );

    // predefines will be stuck into slot 1 by createProgram()
    //    it'll be different for vertex and fragment shader.
    const char* pVSPre = VERTEXPREDEFINES;
    if( m_pVSPredefinitions )
        pVSPre = m_pVSPredefinitions;
    int VSPreLen = (int)strlen( pVSPre );

    const char* pFSPre = FRAGMENTPREDEFINES;
    if( m_pFSPredefinitions )
        pFSPre = m_pFSPredefinitions;
    int FSPreLen = (int)strlen( pFSPre );

    // actually create and compile the shader objects.
    m_ProgramHandle = createProgram( &m_VertexShaderHandle, &m_FragmentShaderHandle,
                                     VSPreLen, pVSPre, FSPreLen, pFSPre,
                                     numchunks, pStrings, pLengths );

    if( m_ProgramHandle == 0 )
    {
        LOGError( "Failed to create program from %s\n", m_pFile->m_FullPath );
    }

    delete[] pStrings;
    delete[] pLengths;

    // TODO: fix support for separate vert/frag files (rather than glsl files).
//    if( m_pFilePixelShader == 0 )
//    {
//#if MYFW_WP8
//        m_ProgramHandle = createProgram( m_pFile->m_FileLength, m_pFile->m_pBuffer,
//                                         m_pFile->m_FileLength, m_pFile->m_pBuffer,
//                                         &m_VertexShaderHandle, &m_FragmentShaderHandle,
//                                         VSPreLen, pVSPre, FSPreLen, pFSPre,
//                                         0 );
//#else
//        m_ProgramHandle = createProgram( m_pFile->m_FileLength, m_pFile->m_pBuffer,
//                                         m_pFile->m_FileLength, m_pFile->m_pBuffer,
//                                         &m_VertexShaderHandle, &m_FragmentShaderHandle,
//                                         VSPreLen, pVSPre, FSPreLen, pFSPre,
//                                         g_ShaderPassDefines[m_PassType] );
//#endif
//    }
//    else
//    {
//#if MYFW_WP8
//        m_ProgramHandle = createProgram( m_pFile->m_FileLength, m_pFile->m_pBuffer,
//                                         m_pFilePixelShader->m_FileLength, m_pFilePixelShader->m_pBuffer,
//                                         &m_VertexShaderHandle, &m_FragmentShaderHandle,
//                                         0, 0, 0, 0,
//                                         0 );
//#else
//        m_ProgramHandle = createProgram( m_pFile->m_FileLength, m_pFile->m_pBuffer,
//                                         m_pFilePixelShader->m_FileLength, m_pFilePixelShader->m_pBuffer,
//                                         &m_VertexShaderHandle, &m_FragmentShaderHandle,
//                                         0, 0, 0, 0,
//                                         g_ShaderPassDefines[m_PassType] );
//#endif
//    }

    if( m_ProgramHandle == 0 )
    {
        LOGError( LOGTag, "Could not create program.\n");
        m_ShaderFailedToCompile = true;
        return false;
    }

    return true;
}

void BaseShader::InitializeAttributeArray(GLint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
    if( index != -1 )
    {
        MyEnableVertexAttribArray( index );
        glVertexAttribPointer( index, size, type, normalized, stride, pointer );
    }
    else
    {
        //LOGInfo( LOGTag, "Shader_Base - Failed to initialize vertex attribute array - %d\n", index );
    }
}

void BaseShader::DisableAttributeArray(GLint index)
{
    if( index != -1 )
    {
        MyDisableVertexAttribArray( index );

        // TODO: set this attribute override value in the MyMesh object.
        glVertexAttrib3f( index, 0, 1, 0 );
    }
}

void BaseShader::DeactivateShader(BufferDefinition* vbo)
{
}

void ShaderManager::AddShader(BaseShader* pShader)
{
    m_ShaderList.AddTail( pShader );
}

void ShaderManager::InvalidateAllShaders(bool cleanglallocs)
{
    glUseProgram(0);

    for( CPPListNode* pNode = m_ShaderList.GetHead(); pNode; pNode = pNode->GetNext() )
    {
        BaseShader* pShader = (BaseShader*)pNode;
        pShader->Invalidate( cleanglallocs );
#if MYFW_WINDOWS && _DEBUG
        // force a reload from disk
        if( pShader->m_pFile )
        {
            g_pFileManager->ReloadFile( pShader->m_pFile );
        }
#endif
    }
}
