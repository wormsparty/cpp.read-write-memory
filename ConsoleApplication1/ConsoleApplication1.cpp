#include <iostream>
#include <Windows.h>
#include <vector>

static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) 
{
	auto windowNames = (std::vector<char*>*)lparam;
	int length = GetWindowTextLength(hWnd);
	char* buffer = new char[length + 1];
	GetWindowTextA(hWnd, buffer, length + 1);

	if (IsWindowVisible(hWnd) && length != 0) 
	{
		std::cout << windowNames->size() << ":  " << buffer << std::endl;
		windowNames->push_back(buffer);
	}

	return TRUE;
}

int main()
{
	std::vector<char*> windowNames;
	EnumWindows(enumWindowCallback, (LPARAM)&windowNames);

	int chosenAppIndex;
	std::cout << "Which application?" << std::endl << "> " << std::flush;
	std::cin >> chosenAppIndex;

	const LPWORD pointer = (LPWORD)0x0172D6F0;
	int32_t readValue = 0;

	HWND hwnd = FindWindowA(NULL, windowNames[chosenAppIndex]);

	if (hwnd == NULL)
	{
		std::cout << "Can't find application " << windowNames[chosenAppIndex] << std::endl;
		exit(1);
	}

	DWORD procId;
	GetWindowThreadProcessId(hwnd, &procId);
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

	if (procId == NULL)
	{
		std::cout << "Can't get process" << std::endl;
		exit(1);
	}

	ReadProcessMemory(handle, pointer, &readValue, sizeof(readValue), 0);

	std::cout << "You have " << readValue << " things at that address. Let's double that value!" << std::endl;
	readValue *= 2;
	WriteProcessMemory(handle, pointer, &readValue, sizeof(readValue), 0);

	std::cout << "Tada!" << std::endl;

	return 0;
}