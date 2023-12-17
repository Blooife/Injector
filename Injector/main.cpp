#include "main.h"

int WINAPI WinMain(
    HINSTANCE imageBaseAddress,
    HINSTANCE prevImageBaseAddress,
    LPSTR cmdLine,
    INT cmdShow
)
{
    Application* ap = new Application;
    ap->instanceHandle = imageBaseAddress;

    mainHandle = ap->InitializeAppSystem(ap->instanceHandle, "Dll Injector", cmdShow);
    if (!(mainHandle)) { MessageBoxA(nullptr, "Cannot create the window", "Unexpected error!", MB_OK | MB_ICONSTOP); return EXIT_FAILURE; }
    
    MSG message;

    while (GetMessage(&message, 0, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return 0;
}

