/*

	SUBJECT:        GTA San andreas Y-Axis senstivity fix (Sync with X)
        VERSION:        1.0.0

*/

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <Psapi.h>
#pragma comment (lib, "psapi.lib")

size_t r_wcsstr(const wchar_t* str, const wchar_t* search)
{
    for (size_t i = wcslen(str) - wcslen(search); i > 0; --i)
    {
        if (wcsstr(str + i, search) != NULL)
            return i + 1;
    }

    return -1;
}

bool GetProcessID(const std::wstring& name, DWORD& pid)
{
    DWORD aProcesses[1024]{ 0 };
    DWORD cbNeeded{ 0 };
    DWORD cProcesses{ 0 };

    unsigned int i;

    if (EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded) == 0)
        return false;

    cProcesses = cbNeeded / sizeof(DWORD);

    for (i = 0; i < cProcesses; i++)
    {
        WCHAR module_name[MAX_PATH]{ 0 };
        HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

        if (process == NULL ||
            GetProcessImageFileNameW(process, module_name, sizeof(module_name) / sizeof(WCHAR)) == 0)
            continue;

        size_t pos = r_wcsstr(module_name, name.c_str());

        if (pos != -1)
        {
            pid = aProcesses[i];
            return true;
        }
    }

    return false;
}

int main()
{
    DWORD ProcessID{ 0 };
    DWORD XSenstivity = 0xB6EC1C;
    DWORD YSenstivity = 0xB6EC18;
    float XSensValue = 0;
    GetProcessID(L"gta_sa.exe", ProcessID);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProcessID);
	if (hProcess == NULL)
	{
		MessageBoxA(NULL, "Game not found!\nRun the script as an Administrator or open your Game.", "CuteLittleScript", MB_ICONERROR);
		return 1;
	}
    std::cout << "Process ID That We Detected: " << ProcessID;
    while (true)
	{
		Sleep(1000);
		ReadProcessMemory(hProcess, (LPCVOID)(XSenstivity), &XSensValue, 4, NULL);
		WriteProcessMemory(hProcess, (LPVOID)(YSenstivity), &XSensValue, 4, NULL);
	}

}
