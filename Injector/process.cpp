#include "process.h"
#include "log.h"
#include <string>
#include <sstream>
#include <WtsApi32.h>

void Process::GetProcessList(HWND processListBoxHandle)
{
    HANDLE processHandleSnap;
    PROCESSENTRY32W pe32;

    memset(&pe32, 0x00, sizeof(pe32));

    pe32.dwSize = sizeof(PROCESSENTRY32W);

    processHandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    Process32FirstW(processHandleSnap, &pe32);

    char pid[8];
    int processNameLength;

    for (UINT v0 = 0; Process32NextW(processHandleSnap, &pe32) != 0x00; v0++) {
        memset(&processNameLength, 0x00, sizeof(int));
        // type conversion HEX -> DEC
        _ultoa_s(pe32.th32ProcessID, pid, 8, 10);
        memcpy((void*)(pe32.szExeFile + lstrlenW(pe32.szExeFile)), "(", 1);

        processNameLength = lstrlenW(pe32.szExeFile);
        for (int v1 = 0; v1 < strlen(pid); v1++) {
            memcpy((void*)(pe32.szExeFile + processNameLength + v1), (pid + v1), 1);
        }

        memcpy((void*)(pe32.szExeFile + lstrlenW(pe32.szExeFile)), ")", 1);

        // Send Strings to list box
        int index = SendMessageW(processListBoxHandle, LB_ADDSTRING, 0, (LPARAM)pe32.szExeFile);
        SendMessageA(processListBoxHandle, LB_SETITEMDATA, index, pe32.th32ProcessID);
    }
    SetFocus(processListBoxHandle);
    CloseHandle(processHandleSnap);
}

BOOL Process::GetProcessModules(HANDLE processHandle, HWND logBoxHandle, char* dllPath)
{
    Log log;
    HMODULE processHandleModules[1024];
    DWORD needed = 0;
    char cModuleName[MAX_PATH], dp[MAX_PATH];

    memcpy(dp, dllPath, MAX_PATH);
    memset((dp + strlen(dp)), 0x00, MAX_PATH - strlen(dp));
    memset(&needed, 0x00, sizeof(DWORD));
    memset(&processHandleModules, 0x00, sizeof(HMODULE) * 1024);

    EnumProcessModules(processHandle, processHandleModules, sizeof(processHandleModules), &needed);
    for (unsigned int v0 = 0; v0 < (needed / sizeof(HMODULE)); v0++) {
        if (GetModuleFileNameExA(processHandle, processHandleModules[v0], cModuleName, sizeof(cModuleName) / sizeof(TCHAR))
            && CompareStringA(LOCALE_ALL, NORM_IGNORECASE, cModuleName, strlen(cModuleName), dp, strlen(dp)) == CSTR_EQUAL) {
            return EXIT_SUCCESS;
        }
        memset(cModuleName, 0x00, sizeof(cModuleName));
    }
    return EXIT_SUCCESS;
}