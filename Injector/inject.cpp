#include "inject.h"
#include "log.h"
#include <tlhelp32.h>
#include <filesystem>

const wchar_t* ExtractDllFileName(const wchar_t* fullPath) {
	const wchar_t* lastSlash = wcsrchr(fullPath, L'\\');
	if (lastSlash != nullptr) {
		return lastSlash + 1;
	}
	return fullPath;

}

DWORD_PTR GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (snapshot == INVALID_HANDLE_VALUE) {
		return 0; // Error
	}

	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(snapshot, &moduleEntry)) {
		do {
			if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
				CloseHandle(snapshot);
				return (DWORD_PTR)moduleEntry.modBaseAddr;
			}
		} while (Module32Next(snapshot, &moduleEntry));
	}

	CloseHandle(snapshot);
	return 0; // Module not found
}

BOOL Injection::NativeInjection()
{
	Log* log = new Log;
	HANDLE threadHandle;
	LPVOID loadLibraryAddress;
	LPVOID dllPathVirtualAddress;

	log->LogOutputW(logBoxHandle, TEXT("!!!!BEGIN INJECTION PROCESS!!!!\r\n"));
	if (dllPath == NULL || pid == 0) {
		log->LogOutputW(logBoxHandle, TEXT("[ERROR] Need to choose dll or process..."));
		return EXIT_FAILURE;
	}
	log->LogOutputW(logBoxHandle, TEXT("[INFO] Open process handle..."));
	processHandle = OpenProcess(
		PROCESS_ALL_ACCESS |
		PROCESS_CREATE_THREAD |
		PROCESS_CREATE_PROCESS |
		PROCESS_VM_WRITE |
		PROCESS_VM_READ |
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION
		, (BOOL)0, pid);

	if (processHandle == (void*)0x00) {
		log->LogOutputW(logBoxHandle, TEXT("Failed!\r\n[ERROR] Failed to open the chosen process\r\n"));
		return EXIT_FAILURE;
	}log->LogOutputW(logBoxHandle, TEXT("Success!\r\n[INFO] processHandle -> "));
	log->LogOutputAddressA(logBoxHandle, processHandle);
	Sleep(1000);
	const wchar_t* dllName = ExtractDllFileName(wDllPath);
	DWORD_PTR dllBaseAddress = GetModuleBaseAddress(pid, dllName);
	if (dllBaseAddress != 0) {
		log->LogOutputW(logBoxHandle, TEXT("\r\n [INFO] Dll already injected. "));
		CallFunction();
		return EXIT_SUCCESS;
	}
	log->LogOutputW(logBoxHandle, TEXT("\r\n[INFO] Get LoadLibraryA address from kernel32.dll..."));
	loadLibraryAddress = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");
	if (loadLibraryAddress == (void*)0x00) {
		log->LogOutputW(logBoxHandle, TEXT("Failed!\r\n Failed to get LoadLibraryA function address on kernel32.dll\r\n"));
		return EXIT_FAILURE;
	}log->LogOutputW(logBoxHandle, TEXT("Success!\r\n[INFO] loadLibraryAddress -> "));
	log->LogOutputAddressA(logBoxHandle, loadLibraryAddress);

	Sleep(1000);
	log->LogOutputW(logBoxHandle, TEXT("\r\n[INFO] VirtualAllocEx..."));
	dllPathVirtualAddress = VirtualAllocEx(processHandle, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (dllPathVirtualAddress == (void*)0x00) {
		log->LogOutputW(logBoxHandle, TEXT("Failed!\r\n Failed to allocate virtual memory area inside chosen process\r\n"));
		return EXIT_FAILURE;
	}log->LogOutputW(logBoxHandle, TEXT("Success!\r\n[INFO] dllPathVirtualAddress -> "));
	log->LogOutputAddressA(logBoxHandle, dllPathVirtualAddress);

	Sleep(1000);
	log->LogOutputW(logBoxHandle, TEXT("\r\n[INFO] WriteProcessMemory..."));
	if (WriteProcessMemory(processHandle, dllPathVirtualAddress, dllPath, MAX_PATH, nullptr) == 0) {
		log->LogOutputW(logBoxHandle, TEXT("Failed!\r\n Failed write dllPath to dllPathVirtualAddress\n"));
		return EXIT_FAILURE;
	}log->LogOutputW(logBoxHandle, TEXT("Success!\r\n"));

	Sleep(1000);

	log->LogOutputW(logBoxHandle, TEXT("\r\n[INFO] CreateRemoteThread..."));
	threadHandle = CreateRemoteThread(processHandle, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddress, dllPathVirtualAddress, 0, 0);
	if (threadHandle == (void*)0x00) {
		log->LogOutputW(logBoxHandle, TEXT("Failed!\r\n Failed create thread on chosen process\n"));
		return EXIT_FAILURE;
	}
	
	WaitForSingleObject(threadHandle, INFINITE);

	log->LogOutputW(logBoxHandle, TEXT("Success!\r\n [INFO] threadHandle -> "));
	log->LogOutputAddressA(logBoxHandle, threadHandle);
	log->LogOutputA(logBoxHandle, "\r\n");

	CallFunction();

	VirtualFree(loadLibraryAddress, 0, MEM_RELEASE);
	VirtualFree(dllPathVirtualAddress, 0, MEM_RELEASE);
	CloseHandle(threadHandle);

	return EXIT_SUCCESS;
}
struct P {
	char a;
	char b;
	char c;
	char d;
	char e;
	double y;
};
BOOL Injection::CallFunction() {
	P p;
	int q = sizeof(p);
	Log* log = new Log;
	log->LogOutputW(logBoxHandle, TEXT("\r\n [INFO] BEGIN Calling function "));
	if (funcName[0] == L'\0') {
		log->LogOutputW(logBoxHandle, TEXT("\r\n [INFO] No function chosen "));
		return EXIT_SUCCESS;
	}
	HMODULE libr = LoadLibraryA((LPCSTR)dllPath);
	if (libr == NULL) {
		return EXIT_FAILURE;
	}

	const wchar_t* dllName = ExtractDllFileName(wDllPath);
	DWORD_PTR dllBaseAddress = GetModuleBaseAddress(pid, dllName);
	if (dllBaseAddress == 0) {
		log->LogOutputW(logBoxHandle, TEXT("\r\n [ERROR] Error in getting dll address in remote process. "));
		return EXIT_FAILURE;
	}
	size_t totalSize = 0;
	int chInARow = 0;
	for (int i = 0; i < gElementCount; ++i) {
		if (intArray[i] != 0) {
			totalSize += sizeof(int);
			if(i < gElementCount - 1)
				if (strArray[i + 1] != nullptr || doubleArray[i + 1] != 0.0) {
					totalSize += 4;
			}
		}
		else if (doubleArray[i] != 0.0) {
			totalSize += sizeof(double);
		}
		else if (charArray[i] != '\0') {
			totalSize += sizeof(char);
			chInARow++;
			if (i < gElementCount - 1)
				if (strArray[i + 1] != nullptr || doubleArray[i + 1] != 0.0) {
					chInARow %= 8;
					totalSize += 8 - chInARow;
					chInARow = 0;
				}
				else if (intArray[i + 1] != 0) {
					chInARow %= 4;
					totalSize += 4 - chInARow;
					chInARow = 0;
				}
				else {

				}
		}
		else if (strArray[i] != nullptr) {
			totalSize += sizeof(const char*);
		}
	}
	chInARow = 0;
	// Выделение блока памяти под все значения
	LPVOID remoteBuffer = VirtualAllocEx(processHandle, NULL, totalSize, MEM_COMMIT, PAGE_READWRITE);

	if (remoteBuffer != nullptr) {
		// Копирование значений в выделенный блок памяти
		LPBYTE currentAddress = reinterpret_cast<LPBYTE>(remoteBuffer);

		for (int i = 0; i < gElementCount; ++i) {
			if (intArray[i] != 0) {
				WriteProcessMemory(processHandle, currentAddress, &intArray[i], sizeof(int), NULL);
				currentAddress += sizeof(int);
				if (i < gElementCount - 1)
					if (strArray[i + 1] != nullptr || doubleArray[i + 1] != 0.0) {
						currentAddress += 4;
					}
			}
			else if (doubleArray[i] != 0.0) {
				WriteProcessMemory(processHandle, currentAddress, &doubleArray[i], sizeof(double), NULL);
				currentAddress += sizeof(double);
			}
			else if (charArray[i] != '\0') {
				WriteProcessMemory(processHandle, currentAddress, &charArray[i], sizeof(char), NULL);
				currentAddress += sizeof(char);
				chInARow++;
				if (i < gElementCount - 1)
					if (strArray[i + 1] != nullptr || doubleArray[i + 1] != 0.0) {
						chInARow %= 8;
						currentAddress += 8 - chInARow;
						chInARow = 0;
					}
					else if (intArray[i + 1] != 0) {
						chInARow %= 4;
						currentAddress += 4 - chInARow;
						chInARow = 0;
					}
					else {

					}
				//currentAddress += 1;
			}
			else if (strArray[i] != nullptr) {
				// Вычисляем размер строки
				size_t stringSize = strlen(strArray[i]) + 1; // +1 для нулевого символа

				// Выделяем память в удаленном процессе для строки
				LPVOID remoteString = VirtualAllocEx(processHandle, NULL, stringSize, MEM_COMMIT, PAGE_READWRITE);

				if (remoteString != nullptr) {
					// Записываем строку в память удаленного процесса
					if (WriteProcessMemory(processHandle, remoteString, strArray[i], stringSize, NULL)) {
						// Записываем указатель на строку в массив strArray
						strArray[i] = reinterpret_cast<const char*>(remoteString);

						// Записываем указатель в выделенный блок памяти
						WriteProcessMemory(processHandle, currentAddress, &strArray[i], sizeof(const char*), NULL);
						currentAddress += sizeof(const char*);
					}
					else {
						// Обработка ошибки записи строки
						VirtualFreeEx(processHandle, remoteString, 0, MEM_RELEASE);
					}
				}
				else {
					log->LogOutputW(logBoxHandle, TEXT("\r\n [ERROR] Write process memory went wrong "));
					return EXIT_FAILURE;
				}
			}
		}
	}
	else {
		log->LogOutputW(logBoxHandle, TEXT("\r\n [ERROR] allocating memory for params went wrong "));
		return EXIT_FAILURE;
	}	

	int requiredSize = WideCharToMultiByte(CP_UTF8, 0, funcName, -1, NULL, 0, NULL, NULL);

	char* buffer = new char[requiredSize];
	WideCharToMultiByte(CP_UTF8, 0, funcName, -1, buffer, requiredSize, NULL, NULL);

	log->LogOutputW(logBoxHandle, TEXT("\r\n [INFO] function name -> "));
	log->LogOutputA(logBoxHandle, buffer);

	FARPROC localMethodAddr = GetProcAddress(libr, buffer);
	if (localMethodAddr == NULL) {
		log->LogOutputW(logBoxHandle, TEXT("\r\n [ERROR] Error when getting proc address "));
		VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
		CloseHandle(processHandle);
		return EXIT_FAILURE;
	}
	DWORDLONG methodDelta = (DWORDLONG)localMethodAddr - (DWORDLONG)libr;
	DWORDLONG remoteMethodAddr = dllBaseAddress + methodDelta;
	log->LogOutputW(logBoxHandle, TEXT("\r\n [INFO] remoteFunctionAddress -> "));
	log->LogOutputAddressA(logBoxHandle, (LPCVOID)remoteMethodAddr);
	log->LogOutputA(logBoxHandle, "\r\n");
	// Создаем удаленный поток для выполнения вашей функции с передачей адреса параметров
	HANDLE remoteThreadF = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)remoteMethodAddr, (LPVOID)remoteBuffer, 0, NULL);
	if (remoteThreadF == NULL)
	{
		log->LogOutputW(logBoxHandle, TEXT("\r\n [ERROR] Error when creating thread "));
		VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
		CloseHandle(processHandle);
		return EXIT_FAILURE;
	}log->LogOutputW(logBoxHandle, TEXT("Success! [INFO] Successfully created thread "));

	// Ожидаем завершения удаленного потока
	//
	WaitForSingleObject(remoteThreadF, INFINITE);

	DWORD lpExitCode;

	GetExitCodeThread(remoteThreadF, &lpExitCode);
	double y = (double)lpExitCode;
	// Освобождаем ресурсы
	VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
	CloseHandle(remoteThreadF);
	FreeLibrary(libr);
	return EXIT_SUCCESS;
}

BOOL Injection::FreeLibrar()
{
	Log* log = new Log;
	HANDLE threadHandle;
	LPVOID freeLibraryAddress;

	log->LogOutputW(logBoxHandle, TEXT("!!!!BEGIN FREE LIBRARY PROCESS!!!!\r\n"));
	if (dllPath == NULL || pid == 0) {
		log->LogOutputW(logBoxHandle, TEXT("[ERROR] Need to choose dll or process..."));
		return EXIT_FAILURE;
	}
	log->LogOutputW(logBoxHandle, TEXT("[INFO] Open process handle..."));
	processHandle = OpenProcess(
		PROCESS_ALL_ACCESS |
		PROCESS_CREATE_THREAD |
		PROCESS_CREATE_PROCESS |
		PROCESS_VM_WRITE |
		PROCESS_VM_READ |
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION
		, (BOOL)0, pid);

	if (processHandle == (void*)0x00) {
		log->LogOutputW(logBoxHandle, TEXT("Failed!\r\n[ERROR] Failed to open the chosen process\r\n"));
		return EXIT_FAILURE;
	}log->LogOutputW(logBoxHandle, TEXT("Success!\r\n[INFO] processHandle -> "));
	log->LogOutputAddressA(logBoxHandle, processHandle);
	Sleep(1000);
	log->LogOutputW(logBoxHandle, TEXT("\r\n [INFO] Getting address of dll in process "));
	const wchar_t* dllName = ExtractDllFileName(wDllPath);
	DWORD_PTR dllBaseAddress = GetModuleBaseAddress(pid, dllName);
	if (dllBaseAddress == 0) {
		log->LogOutputW(logBoxHandle, TEXT("\r\n [INFO] Dll isn't injected. "));
		return EXIT_FAILURE;
	}
	log->LogOutputW(logBoxHandle, TEXT("Success!\r\n[INFO] dllBaseAddress -> "));
	log->LogOutputAddressA(logBoxHandle,(LPVOID)dllBaseAddress);

	log->LogOutputW(logBoxHandle, TEXT("\r\n[INFO] Get FreeLibrary address from kernel32.dll..."));
	freeLibraryAddress = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "FreeLibrary");
	if (freeLibraryAddress == (void*)0x00) {
		log->LogOutputW(logBoxHandle, TEXT("Failed!\r\n Failed to get FreeLibrary function address on kernel32.dll\r\n"));
		return EXIT_FAILURE;
	}log->LogOutputW(logBoxHandle, TEXT("Success!\r\n[INFO] freeLibraryAddress -> "));
	log->LogOutputAddressA(logBoxHandle, freeLibraryAddress);

	Sleep(1000);
	

	log->LogOutputW(logBoxHandle, TEXT("\r\n[INFO] CreateRemoteThread..."));
	threadHandle = CreateRemoteThread(processHandle, nullptr, 0, (LPTHREAD_START_ROUTINE)freeLibraryAddress, (LPVOID)dllBaseAddress, 0, 0);
	if (threadHandle == (void*)0x00) {
		log->LogOutputW(logBoxHandle, TEXT("Failed!\r\n Failed create thread on chosen process\n"));
		return EXIT_FAILURE;
	}
	
	WaitForSingleObject(threadHandle, INFINITE);
	
	log->LogOutputW(logBoxHandle, TEXT("Success!\r\n [INFO] threadHandle -> "));
	log->LogOutputAddressA(logBoxHandle, threadHandle);
	log->LogOutputA(logBoxHandle, "\r\n");

	VirtualFree(freeLibraryAddress, 0, MEM_RELEASE);
	CloseHandle(threadHandle);

	return EXIT_SUCCESS;
}