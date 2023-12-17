#pragma once
#ifndef _LOG_H
#define _LOG_H

#include <windows.h>

namespace Ui { class Log {}; }

class Log {
public:
    void LogOutputA(
        HWND logBoxHandle,
        LPCSTR string
    );
    void LogOutputW(
        HWND logBoxHandle,
        LPCWSTR string
    );
    void LogOutputAddressA(
        HWND logBoxHandle,
        LPCVOID address
    );
};

#endif