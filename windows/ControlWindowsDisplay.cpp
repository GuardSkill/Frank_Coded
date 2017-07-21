#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
static  int VisibleWindowsCount;
static  HWND Hwnd[4096];
static DWORD ProcessId;
BOOL CALLBACK GetVisibleWindowsCountByProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwPid = 0;
	GetWindowThreadProcessId(hwnd, &dwPid); // 获得找到窗口所属的进程    
	if (dwPid == lParam) // 判断是否是目标进程的窗口    
	{
		//SendMessage(hwnd, WM_SHOWWINDOW, TRUE, 0);
		if (IsWindowVisible(hwnd))
		{
			if (VisibleWindowsCount == 4096) return FALSE;
			Hwnd[VisibleWindowsCount++] = hwnd;
		}
	}

	return TRUE;
}
bool EnumVisibleWindows(DWORD PId)
{
	VisibleWindowsCount = 0;
	ProcessId = PId;
	EnumWindows(GetVisibleWindowsCountByProc, PId);    //get visible windows handle by processId
	return TRUE;
}
bool HideWindows()
{
	int i = VisibleWindowsCount;
	while (i >= 1)
	{
		ShowWindow(Hwnd[--i], SW_HIDE);
	}
	return TRUE;
}
bool ShowWindows()
{
	int i = VisibleWindowsCount;
	while (i >= 1)
	{
		ShowWindow(Hwnd[--i], SW_SHOW);
	}
	return TRUE;
}
int main(int argc, char* argv[])
{
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, argv[1], -1, NULL, 0);
	LPWSTR sz = new wchar_t[dwNum+1];
	if (!sz)
	{
		delete[] sz;
	}
	MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, argv[1], -1, sz, dwNum);
	printf("%ls", sz);
	//LPWSTR sz = L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe";
	STARTUPINFO sui = { sizeof(sui) };
	PROCESS_INFORMATION pi;
	BOOL bRet = ::CreateProcess(sz,   //program name
		NULL,                       //command parameter
		NULL,                       //default proccess securiry
		NULL,						//default thread security
		FALSE,                      //the father procces's handle can't be heir 
		NULL,        //build a new console window for new proccess  flags
		NULL,						//father proccess's environment
		NULL,						//father proccess's driver and directory
		&sui, &pi);
	HWND hWnd = NULL;
	getchar();
	EnumVisibleWindows(pi.dwProcessId);
	HideWindows();
	getchar();
	ShowWindows();
	::CloseHandle(pi.hProcess);  //close the process core object handle
	printf("新进程Id号：%d\n", pi.dwProcessId);
	printf("线程id：%d\n", pi.dwThreadId);
	return 0;
}
