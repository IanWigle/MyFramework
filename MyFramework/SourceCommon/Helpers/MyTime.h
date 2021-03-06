//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MyTime_H__
#define __MyTime_H__

// realtime is since Jan 1, 1970 generally. not realtime might be from system boot or other. who knows.
// needs serious testing, realtime is currently only used on blackberry and android.
//   might even be realtime on those and quite possibly realtime on other platforms.
double MyTime_GetSystemTime(bool realtime = false);
double MyTime_GetRunningTime();

// this is just a global, it's up to the game code to keep it up to date.
extern double g_UnpausedTime;
double MyTime_GetUnpausedTime();

#endif //__MyTime_H__
