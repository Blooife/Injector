#pragma once
#ifndef _INJECT_H
#define _INJECT_H


#include "windows.h"

namespace Core { class Injection {}; }

class Injection {
public:
    HANDLE processHandle;
    DWORD pid;
    LPCVOID dllPath;
    HWND logBoxHandle;
    WCHAR funcName[64];

    wchar_t wDllPath[MAX_PATH];
    int intArray[10];
    const char* strArray[10];
    double doubleArray[10];
    char charArray[10];
    int gElementCount = 1;

    BOOL GetProcessList();
    BOOL NativeInjection();
    BOOL CallFunction();
    BOOL FreeLibrar();
};
#endif
