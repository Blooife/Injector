#include "func.h"

void Func::GetFuncList(HWND funcListBoxHandle, char dllPath[MAX_PATH]) {
	// Find the module handle of the specified DLL
	
	HMODULE hModule = LoadLibraryA((LPCSTR)dllPath);
	if (hModule == NULL) {
		return;
	}

	// Enumerate exported functions
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);

	if (ntHeaders->Signature == IMAGE_NT_SIGNATURE) {
		PIMAGE_EXPORT_DIRECTORY exportDirectory =
			(PIMAGE_EXPORT_DIRECTORY)((BYTE*)hModule + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

		if (exportDirectory != NULL) {
			DWORD* functionsRVA = (DWORD*)((BYTE*)hModule + exportDirectory->AddressOfFunctions);
			WORD* ordinals = (WORD*)((BYTE*)hModule + exportDirectory->AddressOfNameOrdinals);
			DWORD* namesRVA = (DWORD*)((BYTE*)hModule + exportDirectory->AddressOfNames);

			for (DWORD i = 0; i < exportDirectory->NumberOfFunctions; ++i) {
				FARPROC functionAddress = (FARPROC)((BYTE*)hModule + functionsRVA[i]);

				// Get the function name
				const char* functionName = (const char*)((BYTE*)hModule + namesRVA[ordinals[i]]);
				SendMessageA(funcListBoxHandle, LB_ADDSTRING, 0, (LPARAM)functionName);
			}
		}
	}
	FreeLibrary(hModule);
}