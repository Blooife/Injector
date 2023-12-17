

#include "./log.h"

void Log::LogOutputA(HWND logBoxHandle, LPCSTR string)
{
    int idx = GetWindowTextLengthA(logBoxHandle);

    SendMessageA(logBoxHandle, EM_SETSEL, (WPARAM)idx, (LPARAM)idx);
    SendMessageA(logBoxHandle, EM_REPLACESEL, 0, (LPARAM)string);
}

void Log::LogOutputW(HWND logBoxHandle, LPCWSTR string)
{
    int idx = GetWindowTextLengthW(logBoxHandle);

    SendMessageW(logBoxHandle, EM_SETSEL, (WPARAM)idx, (LPARAM)idx);
    SendMessageW(logBoxHandle, EM_REPLACESEL, 0, (LPARAM)string);
}

void Log::LogOutputAddressA(HWND logBoxHandle, LPCVOID address)
{
    int idx;
    char string[32];

    memset(string, 0x00, sizeof(char) * 32);

    idx = GetWindowTextLengthA(logBoxHandle);
    SendMessageA(logBoxHandle, EM_SETSEL, (WPARAM)idx, (LPARAM)idx);
    SendMessageA(logBoxHandle, EM_REPLACESEL, 0, (LPARAM)"0x");

    _ultoa_s((unsigned long)address, string, 16, 16);

    idx = GetWindowTextLengthA(logBoxHandle);
    SendMessageA(logBoxHandle, EM_SETSEL, (WPARAM)idx, (LPARAM)idx);
    SendMessageA(logBoxHandle, EM_REPLACESEL, 0, (LPARAM)string);
}
