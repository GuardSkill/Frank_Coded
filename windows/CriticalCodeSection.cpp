#include <stdio.h>
#include <windows.h>
#include <process.h>
LONG g_cnt1=0;
LONG g_cnt2=0;
BOOL g_bool_countine = TRUE;
CRITICAL_SECTION g_cs;
UINT __stdcall first_thread(LPVOID); 
int main(int argc, char *argv[])
{
	UINT uId;
	HANDLE h[2];
	
	::InitializeCriticalSection(&g_cs);
	h[0] = (HANDLE)::_beginthreadex(NULL, 0, first_thread, NULL, 0, &uId);
	h[1] = (HANDLE)::_beginthreadex(NULL, 0, first_thread, NULL, 0, &uId);

	Sleep(1000);
	g_bool_countine = FALSE;
	::WaitForMultipleObjects(2, h, TRUE, INFINITE);
	::CloseHandle(h[0]);
	::CloseHandle(h[1]);

	::DeleteCriticalSection(&g_cs);          //requirement
	printf("g_cnt1=%d\n", g_cnt1);
	printf("g_cnt1=%d\n", g_cnt2);
 	return 0;
}
UINT __stdcall first_thread(LPVOID)
{
	long ret;
	while (g_bool_countine)
	{
		::EnterCriticalSection(&g_cs);
		g_cnt1++;
		g_cnt2++;
		//ret=InterlockedExchangeAdd(&g_cnt1, 1);
		//InterlockedExchangeAdd(&g_cnt2, 1);
		::LeaveCriticalSection(&g_cs);
	}
	return 0;
}