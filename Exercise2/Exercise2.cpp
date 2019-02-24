// Exercise2.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "utils.h"

void HookAmmo(HANDLE hProcess, DWORD baseAddress)
{
	int ammoResetThreshold = 10;

	int firstAddress;
	ReadProcessMemory(hProcess, (LPCVOID)(baseAddress + 0x0010F418), &firstAddress, sizeof(firstAddress), NULL);

	int secondAddress;
	ReadProcessMemory(hProcess, (LPCVOID)(firstAddress + 0x24), &secondAddress, sizeof(secondAddress), NULL);

	int currentAmmo;
	ReadProcessMemory(hProcess, (LPCVOID)secondAddress, &currentAmmo, sizeof(currentAmmo), NULL);

	if (currentAmmo < ammoResetThreshold)
	{
		int nextAmmo = 20;
		WriteProcessMemory(hProcess, (LPVOID)secondAddress, &nextAmmo, sizeof(nextAmmo), NULL);
	}

	ClearConsole();
	printf("Current ammo: %d\n", currentAmmo);
}

int main()
{
	SetConsoleTitle(L"Exercise2");

	DWORD processId = NULL;

	printf("Waiting for Assault Cube ...\n");

	while (!processId) 
	{
		HWND hWnd = FindWindowA(
			NULL,
			"AssaultCube"
		);

		if (hWnd)
		{
			GetWindowThreadProcessId(
				hWnd,
				&processId
			);
		}

		Sleep(10);
	}

	printf("Found it with PID %d\n", processId);

	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,
		FALSE,
		processId
	);

	if (hProcess == NULL) 
	{
		printf("Could not open process, last error: %s\n", GetLastError());
		return EXIT_FAILURE;
	}

	while (true)
	{
		DWORD baseAddress = GetModuleBaseAddress(processId, L"ac_client.exe");

		HookAmmo(hProcess, baseAddress);

		Sleep(20);
	}
}
