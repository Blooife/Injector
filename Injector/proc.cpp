#include "proc.h"

Application* ap = new Application;
Injection* injection = new Injection;
Process* process = new Process;
Log* log = new Log;
Dll* dll = new Dll;
Func* func = new Func;

char dllPath[MAX_PATH];
WCHAR funcName[64];
WCHAR str[64];

int gElementCount = 1;


HWND \
processSelectorDialogHandle,
funcSelectorDialogHandle,
enterParamsDialogHandle,
processComboBoxHandle,
funcComboBoxHandle,
dllComboBoxHandle,
dllDialogHandle,
processDialogHandle,
logBoxHandle,
injectButtonHandle,
processListBoxHandle,
funcListBoxHandle,
hWndResultButton,
hWndCountCombo;
RECT rect;

LRESULT WindowProcedure(
    HWND mainHandle,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    OPENFILENAMEA* ofn;
    PAINTSTRUCT ps;
    HDC hdc = nullptr;

    TCHAR v1[16];
    UINT v0;

    memset(&ofn, 0x00, sizeof(ofn));

    switch (message) {
    case WM_CREATE: {
        //select process
        SetRect(&rect, 30, 70, 450, 30);
        processComboBoxHandle = CreateWindowW(
            TEXT("edit"), nullptr, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT | WS_TABSTOP,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle, nullptr, ap->instanceHandle, nullptr);

        SetRect(&rect, 490, 70, 100, 30);
        processDialogHandle = CreateWindowA(
            "button", "Select", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle, (HMENU)IDC_OPEN_PROCESS_DIALOG, ap->instanceHandle, nullptr);

        //select dll
        SetRect(&rect, 30, 140, 450, 30);
        dllComboBoxHandle = CreateWindowW(
            TEXT("edit"), nullptr, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT | WS_TABSTOP,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle, nullptr, ap->instanceHandle, nullptr);

        SetRect(&rect, 490, 140, 100, 30);
        dllDialogHandle = CreateWindowA(
            "button", "Select", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle, (HMENU)IDC_OPEN_DLL_DIALOG, ap->instanceHandle, nullptr);

        //select function
       SetRect(&rect, 30, 200, 450, 30);
        funcComboBoxHandle = CreateWindowW(
            TEXT("edit"), nullptr, WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT | WS_TABSTOP,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle, nullptr, ap->instanceHandle, nullptr);
        SetRect(&rect, 490, 200, 100, 30);
        dllDialogHandle = CreateWindowA(
            "button", "Select", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle, (HMENU)IDC_OPEN_FUNC_DIALOG, ap->instanceHandle, nullptr);

        SetRect(&rect, 30, 330, 560, 200);

        logBoxHandle = CreateWindowA(
            "edit", nullptr,
            WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_READONLY |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle,
            0,
            ap->instanceHandle, nullptr);

        SetRect(&rect, 30, 550, 275, 35);
        injectButtonHandle = CreateWindowA(
            "button", "Free library", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle, (HMENU)IDC_ADVENCED_CLICK_EVENT, ap->instanceHandle, nullptr);

        SetRect(&rect, 315, 550, 275, 35);
        injectButtonHandle = CreateWindowA(
            "button", "Inject", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle, (HMENU)IDC_INJECT_CLICK_EVENT, ap->instanceHandle, nullptr);

        SetRect(&rect, 30, 250, 275, 35);
        injectButtonHandle = CreateWindowA(
            "button", "Enter params", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            rect.left, rect.top, rect.right, rect.bottom,
            mainHandle, (HMENU)IDC_OPEN_PARAMS_DIALOG, ap->instanceHandle, nullptr);

        break;
    }
    case WM_PAINT: {
        HFONT hFont;
        hdc = BeginPaint(mainHandle, &ps);

        SetRect(&rect, 30, 10, 50, 20);
        hFont = CreateFont(30, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, 0, 0, \
            DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
        if (hFont != nullptr) {
            SelectObject(hdc, hFont);
            DrawText(hdc, TEXT("dll injector"), -1, &rect, DT_NOCLIP);
        }

        SetRect(&rect, 30, 40, 50, 40);
        hFont = CreateFont(33, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, 0, 0, \
            DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
        if (hFont != nullptr) {
            SelectObject(hdc, hFont);
            DrawText(hdc, TEXT("Victim process"), -1, &rect, DT_NOCLIP);
        }
        SetRect(&rect, 30, 100, 50, 40);
        hFont = CreateFont(33, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, 0, 0, \
            DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
        if (hFont != nullptr) {
            SelectObject(hdc, hFont);
            DrawText(hdc, TEXT("Payload Dll"), -1, &rect, DT_NOCLIP);
        }
        SetRect(&rect, 30, 170, 50, 40);
        hFont = CreateFont(33, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, 0, 0, \
            DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
        if (hFont != nullptr) {
            SelectObject(hdc, hFont);
            DrawText(hdc, TEXT("Function"), -1, &rect, DT_NOCLIP);
        }

        DeleteObject(hFont);
        EndPaint(mainHandle, &ps);
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_OPEN_DLL_DIALOG: {
            ofn = dll->OpenDllSelector();
            if (ofn == nullptr) {
                return EXIT_FAILURE;
            }
            memcpy(dllPath, ofn->lpstrFile, MAX_PATH);
            log->LogOutputW(logBoxHandle, TEXT("[DLL] "));
            wchar_t wDllPath[MAX_PATH];
            int nLen = MultiByteToWideChar(CP_ACP, 0, dllPath, sizeof(dllPath), nullptr, 0);
            MultiByteToWideChar(CP_ACP, 0, dllPath, sizeof(dllPath), wDllPath, nLen);
            wcscpy_s(injection->wDllPath, wDllPath);
            log->LogOutputW(logBoxHandle, wDllPath);
            log->LogOutputW(logBoxHandle, TEXT("\r\n"));
            SetWindowTextA(dllComboBoxHandle, dllPath);
            return EXIT_SUCCESS;
            break;
        }
        case IDC_OPEN_PROCESS_DIALOG: {
            processSelectorDialogHandle = CreateWindowExA(
                0,
                ap->classNameSelectProcess,
                "Select process",
                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
                mainHandle, 0, ap->instanceHandle, nullptr);
            ap->SetWindowPositionToCenter(processSelectorDialogHandle);
            ShowWindow(processSelectorDialogHandle, SW_SHOW);
            break;
        }
        case IDC_OPEN_PARAMS_DIALOG: {
            enterParamsDialogHandle = CreateWindowExA(
                0,
                ap->classNameEnterParams,
                "Enter Params",
                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
                mainHandle, 0, ap->instanceHandle, nullptr);
            ap->SetWindowPositionToCenter(enterParamsDialogHandle);
            ShowWindow(enterParamsDialogHandle, SW_SHOW);
            break;
        }
        case IDC_OPEN_FUNC_DIALOG: {
            funcSelectorDialogHandle = CreateWindowExA(
                0,
                ap->classNameSelectFunc,
                "Select func",
                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
                mainHandle, 0, ap->instanceHandle, nullptr);
            ap->SetWindowPositionToCenter(funcSelectorDialogHandle);
            ShowWindow(funcSelectorDialogHandle, SW_SHOW);
            break;
        }
        case IDC_ADVENCED_CLICK_EVENT: {
            injection->pid = ap->victimProcessId;
            injection->dllPath = dllPath;
            injection->logBoxHandle = logBoxHandle;
            if (!injection->FreeLibrar())
            {
                log->LogOutputW(logBoxHandle, TEXT("\n\r[!]  Freed dll successfully!\n"));
            }
            else {
                log->LogOutputW(logBoxHandle, TEXT("\n\r[!] Something went wrong\n"));
            }
            CloseHandle(injection->processHandle);
            break;
        }
        case IDC_INJECT_CLICK_EVENT: {
            injection->pid = ap->victimProcessId;
            if (dllPath[0] != '\0')
                injection->dllPath = dllPath;
            injection->logBoxHandle = logBoxHandle;
            if (!(injection->NativeInjection()))
            {
                if (!(process->GetProcessModules(injection->processHandle, logBoxHandle, dllPath))) {
                    log->LogOutputW(logBoxHandle, TEXT("\n\r[!]  DLL injected successfuly!\n"));
                }
                else {
                    log->LogOutputW(logBoxHandle, TEXT("\n\r[!] DLL not injected! Something went wrong\n"));
                }
            }
            SendMessageA(funcListBoxHandle, LB_ADDSTRING, 0, (LPARAM)"\0");
            CloseHandle(injection->processHandle);
            memset(injection->funcName, 0, sizeof(injection->funcName));
            break;
        }
        }
        break;
    }
    case WM_CLOSE: {
        if (MessageBoxA(mainHandle, "Are you sure close the program?", "No way!", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK) {
            DestroyWindow(mainHandle);
        }
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
    }
    default: {
        return DefWindowProcA(mainHandle, message, wParam, lParam);
    }
           return 0;
    }
}



LRESULT ProcessSelectorDialogProcedure(
    HWND processSelectorDialogHandle,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    WCHAR str[64];
    memset(str, 0x00, sizeof(str));
    switch (message) {
    case WM_CREATE: {
        SetRect(&rect, 30, 30, 400, 360);
        processListBoxHandle = CreateWindowW(
            TEXT("listbox"), nullptr,
            WS_VISIBLE | WS_CHILD | LBS_STANDARD | WS_VSCROLL | ES_AUTOVSCROLL,
            rect.left, rect.top, rect.right, rect.bottom,
            processSelectorDialogHandle, (HMENU)IDC_LISTBOX_CLICK_EVENT, ap->instanceHandle, nullptr);

        SetRect(&rect, 30, 395, 400, 30);
        CreateWindowW(
            TEXT("button"), TEXT("Open"),
            WS_VISIBLE | WS_CHILD,
            rect.left, rect.top, rect.right, rect.bottom,
            processSelectorDialogHandle, (HMENU)IDC_OPEN_PROCESS_CLICK_EVENT, ap->instanceHandle, nullptr);

        process->GetProcessList(processListBoxHandle);
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_LISTBOX_CLICK_EVENT: {
            switch (HIWORD(wParam)) {
            case LBN_DBLCLK:
                int index = SendMessage((HWND)lParam, LB_GETCARETINDEX, 0, 0);
                SendMessage((HWND)lParam, LB_GETTEXT, (LPARAM)index, (WPARAM)str);

                log->LogOutputW(logBoxHandle, TEXT("[PROCESS] "));
                log->LogOutputW(logBoxHandle, str);
                log->LogOutputW(logBoxHandle, TEXT("\r\n"));
                SetWindowTextW(processComboBoxHandle, str);
                ap->victimProcessId = SendMessage((HWND)lParam, LB_GETITEMDATA, (WPARAM)index, 0);
                DestroyWindow(processSelectorDialogHandle);
                break;
            } break;
        }
        case IDC_OPEN_PROCESS_CLICK_EVENT: {
            int index = SendMessage((HWND)processListBoxHandle, LB_GETCARETINDEX, 0, 0);
            SendMessage((HWND)processListBoxHandle, LB_GETTEXT, (LPARAM)index, (WPARAM)str);

            log->LogOutputW(logBoxHandle, TEXT("[PROCESS] "));
            log->LogOutputW(logBoxHandle, str);
            log->LogOutputW(logBoxHandle, TEXT("\r\n"));
            SetWindowTextW(processComboBoxHandle, str);
            ap->victimProcessId = SendMessage((HWND)processListBoxHandle, LB_GETITEMDATA, (WPARAM)index, 0);
            DestroyWindow(processSelectorDialogHandle);
            break;
        } break;
        } break;
    }
    default: {
        if (message == WM_CLOSE || message == WM_DESTROY) { DestroyWindow(processSelectorDialogHandle); }
        return DefWindowProcA(processSelectorDialogHandle, message, wParam, lParam);
    }
           return 0;
    }
}

LRESULT FuncSelectorDialogProcedure(
    HWND funcSelectorDialogHandle,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{    
    memset(str, 0x00, sizeof(str));
    switch (message) {
    case WM_CREATE: {
        SetRect(&rect, 30, 30, 400, 360);
        funcListBoxHandle = CreateWindowW(
            TEXT("listbox"), nullptr,
            WS_VISIBLE | WS_CHILD | LBS_STANDARD | WS_VSCROLL | ES_AUTOVSCROLL,
            rect.left, rect.top, rect.right, rect.bottom,
            funcSelectorDialogHandle, (HMENU)IDC_LISTBOX_CLICK_EVENT, ap->instanceHandle, nullptr);

        SetRect(&rect, 30, 395, 400, 30);
        CreateWindowW(
            TEXT("button"), TEXT("Ok"),
            WS_VISIBLE | WS_CHILD,
            rect.left, rect.top, rect.right, rect.bottom,
            funcSelectorDialogHandle, (HMENU)IDC_OPEN_PROCESS_CLICK_EVENT, ap->instanceHandle, nullptr);
       // func->dllName = (LPCWSTR)dllPath;
        func->GetFuncList(funcListBoxHandle, dllPath);
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_LISTBOX_CLICK_EVENT: {
            switch (HIWORD(wParam)) {
            case LBN_DBLCLK:
                int index = SendMessage((HWND)lParam, LB_GETCARETINDEX, 0, 0);
                SendMessage((HWND)lParam, LB_GETTEXT, (LPARAM)index, (WPARAM)str);

                log->LogOutputW(logBoxHandle, TEXT("[FUNC] "));
                log->LogOutputW(logBoxHandle, str);
                log->LogOutputW(logBoxHandle, TEXT("\r\n"));
                SetWindowTextW(funcComboBoxHandle, str);
                wcscpy_s(injection->funcName, str);
                DestroyWindow(funcSelectorDialogHandle);
                break;
            } break;
        }
        case IDC_OPEN_PROCESS_CLICK_EVENT: {
            int index = SendMessage((HWND)funcListBoxHandle, LB_GETCARETINDEX, 0, 0);
            SendMessage((HWND)funcListBoxHandle, LB_GETTEXT, (LPARAM)index, (WPARAM)str);

            log->LogOutputW(logBoxHandle, TEXT("[FUNC] "));
            log->LogOutputW(logBoxHandle, str);
            log->LogOutputW(logBoxHandle, TEXT("\r\n"));
            SetWindowTextW(funcComboBoxHandle, str);
            DestroyWindow(funcSelectorDialogHandle);
            break;
        } break;
        } break;
    }
    default: {
        if (message == WM_CLOSE || message == WM_DESTROY) { DestroyWindow(funcSelectorDialogHandle); }
        return DefWindowProcA(funcSelectorDialogHandle, message, wParam, lParam);
    }
           return 0;
    }
}


void CreateControls(HWND hwnd) {
    // Create "Number of Elements" ComboBox
    hWndCountCombo = CreateWindowEx(0, WC_COMBOBOX, L"", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        10, 10, 100, 200, hwnd, (HMENU)IDC_COUNT_COMBO, GetModuleHandle(NULL), NULL);    

    // Populate "Number of Elements" ComboBox
    for (int i = 1; i <= 6; ++i) {
        WCHAR buffer[7];
        _itow_s(i, buffer, 7);
        SendMessage(hWndCountCombo, CB_ADDSTRING, 0, (LPARAM)buffer);
    }
    // Set default selection
    SendMessage(hWndCountCombo, CB_SETCURSEL, 0, 0);
    // Create "Result" Button
    hWndResultButton = CreateWindowEx(0, L"BUTTON", L"Get Result", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        300, 40, 100, 30, hwnd, (HMENU)IDC_RESULT_BUTTON, GetModuleHandle(NULL), NULL);
}

void CreateElementControls(HWND hwnd, int elementCount) {

    for (int i = 0; i < gElementCount; ++i) {
        bool dCombo = DestroyWindow(GetDlgItem(hwnd, IDC_ELEMENT_CONTROLS_START + i));
        bool dEdit = DestroyWindow(GetDlgItem(hwnd, IDC_ELEMENT_CONTROLS_START + 100 + i));
    }    

    // Create new controls
    for (int i = 0; i < elementCount; ++i) {
        // Create Type ComboBox
        HWND hElementTypeCombo = CreateWindowEx(0, WC_COMBOBOX, L"", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
            10, 40 + i * 60, 150, 200, hwnd, (HMENU)(IDC_ELEMENT_CONTROLS_START + i), GetModuleHandle(NULL), NULL);

        // Populate Type ComboBox with possible types
        const wchar_t* elementTypes[] = { L"int", L"char", L"double", L"char pointer"};
        for (int j = 0; j < ARRAYSIZE(elementTypes); ++j) {
            SendMessage(hElementTypeCombo, CB_ADDSTRING, 0, (LPARAM)elementTypes[j]);
        }

        // Set default selection
        SendMessage(hElementTypeCombo, CB_SETCURSEL, 0, 0);

        // Create Value Edit control
        HWND hElementValueEdit = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_BORDER | WS_VISIBLE,
            10, 70 + i * 60, 100, 25, hwnd, (HMENU)(IDC_ELEMENT_CONTROLS_START + 100 + i), GetModuleHandle(NULL), NULL);
    }


    // Update global element count
    gElementCount = elementCount;
    injection->gElementCount = elementCount;
}

void ProcessResult(HWND hwnd) {
    // Clean arrays

    memset(injection->intArray, 0, sizeof(injection->intArray));
    memset(injection->doubleArray, 0, sizeof(injection->doubleArray));
    memset(injection->charArray, 0, sizeof(injection->charArray));

    for (int i = 0; i < gElementCount; ++i) {
        //delete[] injection->strArray[i];
        injection->strArray[i] = nullptr;
    }

    injection->gElementCount = gElementCount;
    // Process result based on the current element count
    for (int i = 0; i < gElementCount; ++i) {
        int typeIndex = SendMessage(GetDlgItem(hwnd, IDC_ELEMENT_CONTROLS_START + i), CB_GETCURSEL, 0, 0);
        char buffer[256];
        GetWindowTextA(GetDlgItem(hwnd, IDC_ELEMENT_CONTROLS_START + 100 + i), buffer, ARRAYSIZE(buffer));

        // Process type and value as needed
        switch (typeIndex) {
        case 0: // int
            if (isdigit(buffer[0]) || (buffer[0] == '-' && isdigit(buffer[1]))) {
                // ¬вод корректен, продолжаем
                log->LogOutputA(logBoxHandle, "Param int: ");
                log->LogOutputA(logBoxHandle, buffer);
                log->LogOutputA(logBoxHandle, "\r\n");
                injection->intArray[i] = atoi(buffer);
            }
            else {
                log->LogOutputA(logBoxHandle, "Wrong input for param int\n");
            }
            break;
        case 1: // char
            if (strlen(buffer) > 0) {
                if (strlen(buffer) == 1) {
                    log->LogOutputA(logBoxHandle, "Param char: ");
                    log->LogOutputA(logBoxHandle, buffer);
                    log->LogOutputA(logBoxHandle, "\r\n");
                    injection->charArray[i] = buffer[0];
                }
                else {
                    log->LogOutputA(logBoxHandle, "Wrong input for param char\n");
                }
            }
            break;
        case 2: {// double
            int dotCount = 0;
            bool wrong = false;
            for (size_t j = 0; j < strlen(buffer); ++j) {
                if (!isdigit(buffer[j])) {
                    if (buffer[j] == '.' && dotCount == 0) {
                        ++dotCount;
                    }
                    else {
                        log->LogOutputA(logBoxHandle, "Wrong input for param double\n");
                        wrong = true;
                        break;
                    }
                }
            }
            if (!wrong) {
                log->LogOutputA(logBoxHandle, "Param double: ");
                log->LogOutputA(logBoxHandle, buffer);
                log->LogOutputA(logBoxHandle, "\r\n");
                injection->doubleArray[i] = atof(buffer);
            }            
            break;
        }
        case 3: // char pointer
            log->LogOutputA(logBoxHandle, "Param char pointer : ");
            log->LogOutputA(logBoxHandle, buffer);
            log->LogOutputA(logBoxHandle, "\r\n");
            injection->strArray[i] = new char[strlen(buffer) + 1]; // +1 дл€ учета завершающего нулевого символа
            //strcpy(injection->strArray[i], buffer);
            injection->strArray[i] = _strdup(buffer);
            break;
        }
    }
}


LRESULT EnterParamsDialogProcedure(HWND enterParamsDialogHandle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        CreateControls(enterParamsDialogHandle);
        CreateElementControls(enterParamsDialogHandle, 1);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_RESULT_BUTTON && HIWORD(wParam) == BN_CLICKED) {
            ProcessResult(enterParamsDialogHandle);
            DestroyWindow(enterParamsDialogHandle);
        }
        else if (LOWORD(wParam) == IDC_COUNT_COMBO && HIWORD(wParam) == CBN_SELCHANGE) {
            // Handle selection change in the "Number of Elements" ComboBox
            int countIndex = SendMessage(hWndCountCombo, CB_GETCURSEL, 0, 0);
            CreateElementControls(enterParamsDialogHandle, countIndex + 1);
            
        }
        break;
    default: {
        if (message == WM_CLOSE || message == WM_DESTROY) { DestroyWindow(enterParamsDialogHandle); }
        return DefWindowProcA(enterParamsDialogHandle, message, wParam, lParam);
    }
           return 0;
    }
}
