#pragma once

// Memory leak debug reference: https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2017

#if defined(_WIN32)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// Replace new with dbg_new to print out file and line number of code which causes memory leak
#define dbg_new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#define EnableMemoryLeakDetection() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#define SetMemoryAllocBreakPoint(n) _CrtSetBreakAlloc(n)
#endif