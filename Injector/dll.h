#pragma once
#ifndef _DLL_H
#define _DLL_H


#include "windows.h"

namespace Core { class Dll {}; }

class Dll {
public:
    OPENFILENAMEA* OpenDllSelector();
};

#endif
