#include "dll.h"

OPENFILENAMEA* Dll::OpenDllSelector()
{
    OPENFILENAMEA ofn;
    char cDllPath[MAX_PATH];

    memset(&ofn, 0x00, sizeof(ofn));
    memset(&cDllPath, 0x00, sizeof(cDllPath));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFile = cDllPath;
    ofn.lpstrFilter = "DLL\0*.dll\0";
    ofn.nFilterIndex = 1;
    ofn.nMaxFileTitle = 0;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn) == 1) {
        return &ofn;
    }
}