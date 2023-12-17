
#ifndef _APP_H
#define _APP_H

#include "proc.h"
#include <windows.h>

namespace Ui { class Application {}; };

class Application {
private:
    WNDCLASSA windowClass;
    RECT rect;
public:
    const LPCSTR classNameMain = "MAIN";
    const LPCSTR classNameSelectProcess = "PROCESS_SELECTOR";
    const LPCSTR classNameSelectFunc = "FUNC_SELECTOR";
    const LPCSTR classNameEnterParams = "ENTER_PARAMS";
    const LPCSTR classNameBackgroundWhite = "BACKGROUND_WHITE";

    DWORD victimProcessId;
    HWND mainHandle;
    HINSTANCE instanceHandle;
    HWND InitializeAppSystem(
        HINSTANCE imageBaseAddress,
        LPCSTR applicationTitle,
        int cmdShow
    );
    void SetWindowPositionToCenter(
        HWND windowHandle
    );
};

#endif#pragma once
