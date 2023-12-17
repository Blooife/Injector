
#include "app.h"

HWND Application::InitializeAppSystem(
    HINSTANCE imageBaseAddress,
    LPCSTR applicationTitle,
    int cmdShow
)
{
    memset(&windowClass, 0x00, sizeof(WNDCLASSA));
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.cbWndExtra = sizeof(PVOID);
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hInstance = imageBaseAddress;
    windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;
    windowClass.lpszClassName = classNameMain;
    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowClass.hIcon = NULL;
    if (!(RegisterClassA(&windowClass))) { return (HWND)0x00; }

    windowClass.lpfnWndProc = (WNDPROC)ProcessSelectorDialogProcedure;
    windowClass.lpszClassName = classNameSelectProcess;

    if (!(RegisterClassA(&windowClass))) { return (HWND)0x00; }

    windowClass.lpfnWndProc = (WNDPROC)FuncSelectorDialogProcedure;
    windowClass.lpszClassName = classNameSelectFunc;

    if (!(RegisterClassA(&windowClass))) { return (HWND)0x00; }

    windowClass.lpfnWndProc = (WNDPROC)EnterParamsDialogProcedure;
    windowClass.lpszClassName = classNameEnterParams;

    if (!(RegisterClassA(&windowClass))) { return (HWND)0x00; }

    SetRect(&rect, 40, 115, 650, 650);
    mainHandle = CreateWindowExA(
        0, classNameMain,
        applicationTitle,
        WS_OVERLAPPED | WS_SYSMENU,
        rect.left, rect.top, rect.right, rect.bottom,
        0, 0, imageBaseAddress, 0);

    SetWindowPositionToCenter(mainHandle);

    if (!(mainHandle)) { return (HWND)0x00; }
    ShowWindow(mainHandle, SW_SHOW);
    return mainHandle;
}

void Application::SetWindowPositionToCenter(
    HWND windowHandle
)
{
    RECT rect;
    GetWindowRect(windowHandle, &rect);
    SetWindowPos(
        windowHandle, 0,
        (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2,
        0, 0,
        SWP_NOZORDER | SWP_NOSIZE
    );
}