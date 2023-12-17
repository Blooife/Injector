
#ifndef _PROC_H
#define _PROC_H

#include <windows.h>
#include "dll.h"
#include "inject.h"
#include "define.h"
#include "./log.h"
#include "./app.h"
#include "process.h"
#include "func.h"
#include <commctrl.h>
#include <wchar.h>

namespace Ui { class Procedure {}; }

LRESULT WindowProcedure(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);
LRESULT ProcessSelectorDialogProcedure(
    HWND processSelectorDialogHandle,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);

LRESULT FuncSelectorDialogProcedure(
    HWND funcSelectorDialogHandle,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);

LRESULT EnterParamsDialogProcedure(
    HWND enterParamsDialogHandle,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);

#endif
