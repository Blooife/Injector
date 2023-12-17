
#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <windowsx.h>
#include <commctrl.h>

#include "process.h"
#include "dll.h"
#include "inject.h"
#include "define.h"
#include "./log.h"
#include "./proc.h"
#include "./app.h"

#define CONTAINER (30)

HWND mainHandle;

int WINAPI WinMain(
    HINSTANCE imageBaseAddress,
    HINSTANCE prevImageBaseAddress,
    LPSTR cmdLine,
    INT cmdShow
);

#endif#pragma once
