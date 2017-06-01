//
// Copyright (c) 2015-2017 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CommonHeader.h"
#include "MyMeshLoader.h"

void MyMesh::LoadMyMesh(const char* buffer, MyList<MySubmesh*>* pSubmeshList, float scale)
{
    checkGlError( "MyMesh::LoadMyMesh" );

    MyAssert( pSubmeshList );
    MyAssert( pSubmeshList->Length() == 0 );

    // get the number of verts/indices/bones.
    unsigned int totalverts = 0;
    unsigned int totalindices = 0;
    unsigned int totalbones = 0;
    unsigned int totalnodes = 0;
    unsigned int totalanimtimelines = 0;

    Vector3 minvert;
    Vector3 maxvert;

    cJSON* root = cJSON_Parse( buffer );

    cJSONExt_GetFloat( root, "InitialScale", &m_InitialScale );
    scale = m_InitialScale;

    // find a line that starts with "#raw" and store it's index.
    unsigned int bufferlen = (int)strlen( buffer );
    unsigned int rawbyteoffset = 0;
    if( bufferlen > 5 )
    {
        for( unsigned int i=0; i<bufferlen; i++ )
        {
            if( i > 1 && strncmp( &buffer[i], "\n#RAW", 5 ) == 0 )
            {
                rawbyteoffset = i+6;
            }
        }
    }

    {
        cJSONExt_GetUnsignedInt( root, "TotalBones", &totalbones );
        cJSONExt_GetUnsignedInt( root, "TotalNodes", &totalnodes );

        // Read in bone info.
        if( totalbones )
        {
            m_BoneNames.AllocateObjects( totalbones );
            m_BoneOffsetMatrices.AllocateObjects( totalbones );
            m_BoneFinalMatrices.AllocateObjects( totalbones );

            cJSON* bones = cJSON_GetObjectItem( root, "Bones" );
            MyAssert( totalbones == (unsigned int)cJSON_GetArraySize( bones ) );
            for( unsigned int i=0; i<totalbones; i++ )
            {
                cJSON* bone = cJSON_GetArrayItem( bones, i );
                if( bone )
                {
                    int namelen = (int)strlen( bone->valuestring );
                    char* bonename = MyNew char[namelen+1];
                    strcpy_s( bonename, namelen+1, bone->valuestring );

                    m_BoneNames.Add( bonename );
                }
            }
        }

        // Read in skeleton's node tree.
        if( totalnodes > 0 )
        {
            m_pSkeletonNodeTree.AllocateObjects( totalnodes );

            cJSON* rootnode = cJSON_GetObjectItem( root, "Nodes" );

            LoadMyMesh_ReadNode( rootnode->child, 0 );
        }

        // Read in the animation timelines.
        cJSON* animarray = cJSON_GetObjectItem( root, "AnimArray" );
        if( animarray )
        {
            totalanimtimelines = cJSON_GetArraySize( animarray );
            if( totalanimtimelines > 0 )
            {
                m_pAnimationTimelines.AllocateObjects( totalanimtimelines );

                for( unsigned int ai=0; ai<totalanimtimelines; ai++ )
                {
                    MyAnimationTimeline* pAnim = MyNew MyAnimationTimeline;
                    m_pAnimationTimelines.Add( pAnim );

                    cJSON* pAnimObj = cJSON_GetArrayItem( animarray, ai );
                    pAnim->ImportFromJSON( pAnimObj );
                }
            }
        }
    }

    cJSON* mesharray = cJSON_GetObjectItem( root, "Meshes" );
    if( mesharray )
    {
        cJSON* mesh = 0;
        if( mesharray )
            mesh = mesharray->child;

        int nummeshes = cJSON_GetArraySize( mesharray );
        pSubmeshList->AllocateObjects( nummeshes );
        for( int i=0; i<nummeshes; i++ )
            pSubmeshList->Add( MyNew MySubmesh() );

        int meshcount = 0;
        while( mesh )
        {
            MyAssert( meshcount < pSubmeshList->Count() );

            MySubmesh* pSubmesh = (*pSubmeshList)[meshcount];

            BufferDefinition** ppVBO = &pSubmesh->m_pVertexBuffer;
            BufferDefinition** ppIBO = &pSubmesh->m_pIndexBuffer;

            MyAssert( ppVBO );
            MyAssert( ppIBO );

            cJSON* jMaterial = cJSON_GetObjectItem( mesh, "Material" );
            if( jMaterial && jMaterial->valuestring )
            {
                MaterialDefinition* pMaterial = g_pMaterialManager->LoadMaterial( jMaterial->valuestring );
                if( pMaterial )
                    pSubmesh->SetMaterial( pMaterial );
                pMaterial->Release();
            }

            cJSONExt_GetUnsignedInt( mesh, "TotalVerts", &totalverts );
            cJSONExt_GetUnsignedInt( mesh, "TotalIndices", &totalindices );

            unsigned int numuvchannels = 0;
            bool hasnormals = false;
            bool hastangents = false;
            bool hasbitangents = false;
            bool hascolor = false;
            unsigned int mostbonesinfluences = 0;

            cJSONExt_GetUnsignedInt( mesh, "VF-uv", &numuvchannels );
            cJSONExt_GetBool( mesh, "VF-normal", &hasnormals );
            cJSONExt_GetBool( mesh, "VF-tangent", &hastangents );
            cJSONExt_GetBool( mesh, "VF-bitangent", &hasbitangents );
            cJSONExt_GetBool( mesh, "VF-color", &hascolor );
            cJSONExt_GetUnsignedInt( mesh, "VF-mostweights", &mostbonesinfluences );

            VertexFormat_Dynamic_Desc* pDesc = g_pVertexFormatManager->GetDynamicVertexFormat( numuvchannels, hasnormals, hastangents, hasbitangents, hascolor, mostbonesinfluences );

            // read this mesh's raw bytes, verts/indices/etc.
            if( rawbyteoffset != 0 )
            {
                int bytesperindex = 4;
                if( totalverts <= 256 )
                    bytesperindex = 1;
                else if( totalverts <= 256*256 )
                    bytesperindex = 2;

                unsigned int vertbuffersize = totalverts * pDesc->stride;
                unsigned int indexbuffersize = totalindices * bytesperindex;
                unsigned char* verts = MyNew unsigned char[vertbuffersize];
                unsigned char* indices = MyNew unsigned char[indexbuffersize];

                // read the raw data:
                {
                    // read vert buffer bytes //(Vertex_XYZUVNorm_RGBA_4Bones*)verts,10
                    memcpy( verts, &buffer[rawbyteoffset], vertbuffersize );
                    rawbyteoffset += vertbuffersize;

                    // scale the verts if requested... should be done at export or not at all.
                    // assumes position is the first attribute... ugh. TODO: rip this out.
                    if( scale != 1.0f )
                    {
                        for( unsigned int i=0; i<totalverts; i++ )
                        {
                            ((float*)(&(verts[pDesc->stride * i])))[0] *= scale;
                            ((float*)(&(verts[pDesc->stride * i])))[1] *= scale;
                            ((float*)(&(verts[pDesc->stride * i])))[2] *= scale;
                        }
                    }

                    for( unsigned int i=0; i<totalverts; i++ )
                    {
                        if( ((float*)(&(verts[pDesc->stride * i])))[0] < minvert.x || i == 0 ) minvert.x = ((float*)(&(verts[pDesc->stride * i])))[0];
                        if( ((float*)(&(verts[pDesc->stride * i])))[1] < minvert.y || i == 0 ) minvert.y = ((float*)(&(verts[pDesc->stride * i])))[1];
                        if( ((float*)(&(verts[pDesc->stride * i])))[2] < minvert.z || i == 0 ) minvert.z = ((float*)(&(verts[pDesc->stride * i])))[2];
                        if( ((float*)(&(verts[pDesc->stride * i])))[0] > maxvert.x || i == 0 ) maxvert.x = ((float*)(&(verts[pDesc->stride * i])))[0];
                        if( ((float*)(&(verts[pDesc->stride * i])))[1] > maxvert.y || i == 0 ) maxvert.y = ((float*)(&(verts[pDesc->stride * i])))[1];
                        if( ((float*)(&(verts[pDesc->stride * i])))[2] > maxvert.z || i == 0 ) maxvert.z = ((float*)(&(verts[pDesc->stride * i])))[2];
                    }

                    // read index buffer bytes
                    memcpy( indices, &buffer[rawbyteoffset], indexbuffersize );
                    rawbyteoffset += indexbuffersize;
                }

                // give verts and indices pointers to BufferDefinition objects, which will handle the delete[]'s
                if( *ppVBO == 0 )
                {
                    *ppVBO = g_pBufferManager->CreateBuffer();
                }

                if( *ppIBO == 0 )
                {
                    *ppIBO = g_pBufferManager->CreateBuffer();
                }

                // The buffer will delete the allocated arrays of verts/indices
                (*ppVBO)->InitializeBuffer( verts, vertbuffersize, GL_ARRAY_BUFFER, GL_STATIC_DRAW, true, 1, VertexFormat_Dynamic, pDesc, "MyMeshLoader", "VBO" );
                (*ppIBO)->InitializeBuffer( indices, indexbuffersize, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, true, 1, bytesperindex, "MyMeshLoader", "IBO" );

                //delete[] verts;
                //delete[] indices;

                MyAssert( pSubmeshList->Count() > 0 );
                pSubmesh->m_VertexFormat = (*ppVBO)->m_VertexFormat;
                pSubmesh->m_NumIndicesToDraw = (*ppIBO)->m_DataSize / (*ppIBO)->m_BytesPerIndex;
            }

            // get the next mesh from the cJSON array.
            mesh = mesh->next;
            meshcount++;
        }

        // read in the rest of the raw data.
        if( rawbyteoffset != 0 )
        {
            // read bone offset matrices
            {
                if( totalbones > 0 )
                {
                    m_BoneOffsetMatrices.BlockFill( &buffer[rawbyteoffset], sizeof(MyMatrix)*totalbones, totalbones );
                    rawbyteoffset += sizeof(MyMatrix)*totalbones;

                    for( unsigned int i=0; i<m_BoneOffsetMatrices.Count(); i++ )
                    {
                        m_BoneOffsetMatrices[i].m41 *= scale;
                        m_BoneOffsetMatrices[i].m42 *= scale;
                        m_BoneOffsetMatrices[i].m43 *= scale;
                    }
                }

                // initialize all the final bone matrices to identity.
                MyMatrix matidentity;
                matidentity.SetIdentity();
                for( unsigned int i=0; i<totalbones; i++ )
                {
                    m_BoneFinalMatrices.Add( matidentity );
                }
            }

            // Read in the node transforms
            for( unsigned int ni=0; ni<totalnodes; ni++ )
            {
                MyAssert( ni < m_pSkeletonNodeTree.Count() );

                // TODO: this line fails on Android(gcc)... no clue why, so did it with a memcpy.
                //m_pSkeletonNodeTree[ni].m_Transform = *(MyMatrix*)&buffer[rawbyteoffset];
                memcpy( &m_pSkeletonNodeTree[ni].m_Transform, &buffer[rawbyteoffset], sizeof(MyMatrix) );

                rawbyteoffset += sizeof(MyMatrix);

                m_pSkeletonNodeTree[ni].m_Transform.m41 *= scale;
                m_pSkeletonNodeTree[ni].m_Transform.m42 *= scale;
                m_pSkeletonNodeTree[ni].m_Transform.m43 *= scale;
            }

            // read animation channels
            for( unsigned int ai=0; ai<totalanimtimelines; ai++ )
            {
                MyAssert( ai < m_pAnimationTimelines.Count() );

                rawbyteoffset += m_pAnimationTimelines[ai]->ImportChannelsFromBuffer( &buffer[rawbyteoffset], scale );
            }
        }
    }

    cJSON_Delete( root );

    // if something failed to load, don't consider the mesh ready.
    // TODO: add better error handling... maybe
    if( pSubmeshList->Count() > 0 )
    {
        Vector3 center = (minvert + maxvert) / 2;
        m_AABounds.Set( center, maxvert - center );

        m_MeshReady = true;
    }
}

int MyMesh::FindBoneIndexByName(char* name)
{
    for( unsigned int i=0; i<m_BoneNames.Count(); i++ )
    {
        if( strcmp( m_BoneNames[i], name ) == 0 )
        {
            return i;
        }
    }

    return -1;
}

void MyMesh::LoadMyMesh_ReadNode(cJSON* pNode, MySkeletonNode* pParentSkelNode)
{
    MySkeletonNode skelnodetoadd;
    int skelnodeindex = m_pSkeletonNodeTree.Count();
    m_pSkeletonNodeTree.Add( skelnodetoadd );

    MyAssert( skelnodeindex < m_pSkeletonNodeTree.Count() );

    MySkeletonNode& skelnode = m_pSkeletonNodeTree[skelnodeindex];

    // Add this node as a child of the parent.
    if( pParentSkelNode )
        pParentSkelNode->m_pChildren.Add( &m_pSkeletonNodeTree[skelnodeindex] );

    char* name = pNode->string;
    MyAssert( name );

    skelnode.m_SkeletonNodeIndex = skelnodeindex;
    skelnode.m_BoneIndex = FindBoneIndexByName( name );

    // add the name.
    int namelen = (int)strlen(name);
    skelnode.m_Name = MyNew char[namelen+1];
    strcpy_s( skelnode.m_Name, namelen+1, name );

    // get count of children.
    unsigned int childcount = cJSONExt_GetDirectChildCount( pNode );

    if( childcount > 0 )
    {
        // allocate enough pointer for each child.
        skelnode.m_pChildren.AllocateObjects( childcount );

        // recurse through the children.
        cJSON* childnode = pNode->child;
        while( childnode )
        {
            LoadMyMesh_ReadNode( childnode, &m_pSkeletonNodeTree[skelnodeindex] );
            childnode = childnode->next;
        }
    }
}

#if MYFW_USING_WX
void MyMesh::SaveAnimationControlFile()
{
    char filename[MAX_PATH];
    m_pSourceFile->GenerateNewFullPathExtensionWithSameNameInSameFolder( ".myaniminfo", filename, MAX_PATH );

    cJSON* root = cJSON_CreateObject();

    cJSON* animarray = cJSON_CreateArray();
    cJSON_AddItemToObject( root, "Anims", animarray );

    for( unsigned int i=0; i<m_pAnimations.Count(); i++ )
    {
        cJSON* anim = cJSON_CreateObject();

        cJSON_AddItemToArray( animarray, anim );

        cJSON_AddStringToObject( anim, "Name", m_pAnimations[i]->m_Name );
        cJSON_AddNumberToObject( anim, "TimelineIndex", m_pAnimations[i]->m_TimelineIndex );
        cJSON_AddNumberToObject( anim, "StartTime", m_pAnimations[i]->m_StartTime );
        cJSON_AddNumberToObject( anim, "Duration", m_pAnimations[i]->m_Duration );
    }

    // dump animarray to disk
    char* jsonstr = cJSON_Print( root );
    cJSON_Delete( root );

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

void MyMesh::LoadAnimationControlFile(const char* buffer)
{
    // if the file doesn't exist, create a single animation for each timeline
    if( buffer == 0 )
    {
        MyAssert( m_pAnimationTimelines.Count() < MAX_ANIMATIONS );

        for( unsigned int i=0; i<m_pAnimationTimelines.Count(); i++ )
        {
            MyAnimation* pAnim = MyNew MyAnimation;
            
            pAnim->SetName( m_pAnimationTimelines[i]->m_Name );
            pAnim->m_TimelineIndex = i;
            pAnim->m_StartTime = 0;
            pAnim->m_Duration = m_pAnimationTimelines[i]->m_Duration;

            m_pAnimations.Add( pAnim );
        }

        // only save if the user adds animations via the editor interface.
        //SaveAnimationControlFile();
    }
    else
    {
        cJSON* root = cJSON_Parse( buffer );

        if( root )
        {
            cJSON* animarray = cJSON_GetObjectItem( root, "Anims" );

            int numanims = cJSON_GetArraySize( animarray );
            for( int i=0; i<numanims; i++ )
            {
                cJSON* anim = cJSON_GetArrayItem( animarray, i );

                MyAnimation* pAnim = MyNew MyAnimation;

                cJSON* obj = cJSON_GetObjectItem( anim, "Name" );
                if( obj )
                    pAnim->SetName( obj->valuestring );
                cJSONExt_GetInt( anim, "TimelineIndex", &pAnim->m_TimelineIndex );
                cJSONExt_GetFloat( anim, "StartTime", &pAnim->m_StartTime );
                cJSONExt_GetFloat( anim, "Duration", &pAnim->m_Duration );

                m_pAnimations.Add( pAnim );
            }
        }

        cJSON_Delete( root );
    }
}
