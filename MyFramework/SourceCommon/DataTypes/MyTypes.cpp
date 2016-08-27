//
// Copyright (c) 2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CommonHeader.h"

void TestMyTypeSizes()
{
    bool ok;

    sizeof( int32 ) == 4  ? ok = true : ok = false; MyAssert( ok ); if( !ok ) LOGError( LOGTag, "TYPE TEST FAILED!!!!\n" );
    sizeof( uint32 ) == 4 ? ok = true : ok = false; MyAssert( ok ); if( !ok ) LOGError( LOGTag, "TYPE TEST FAILED!!!!\n" );
    sizeof( int64 ) == 8  ? ok = true : ok = false; MyAssert( ok ); if( !ok ) LOGError( LOGTag, "TYPE TEST FAILED!!!!\n" );
    sizeof( uint64 ) == 8 ? ok = true : ok = false; MyAssert( ok ); if( !ok ) LOGError( LOGTag, "TYPE TEST FAILED!!!!\n" );
}
