#pragma once

#include <Windows.h>
#include <TlHelp32.h>

void ClearConsole();

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
