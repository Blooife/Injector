#pragma once
#ifndef _PROCESS_H
#define _PROCESS_H

#include "windows.h"
#include <tlhelp32.h>
#include <tchar.h>
#include <psapi.h>

namespace Core { class Process {}; }

class Process {
public:
    void GetProcessList(
      HWND processListBoxHandle
    );

    BOOL GetProcessModules(
      HANDLE processHandle,
      HWND logBoxHandle,
      char* dllPath
    );
};

#endif
