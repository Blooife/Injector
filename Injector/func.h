#pragma once
#include <Windows.h>
namespace Core { class Func {}; }

class Func {
public:
    void GetFuncList(HWND funcListBoxHandle, char dllPath[MAX_PATH]);
};