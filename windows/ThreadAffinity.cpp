#include <iostream>
#include <Windows.h>
#include <bitset>  
using namespace std;
int main()
{
	DWORD p_mask, s_mask;
	HANDLE h = GetCurrentProcess();
	GetProcessAffinityMask(h, &p_mask, &s_mask);
	cout << hex << p_mask << endl;               //16 bit
	cout << "p_mask:" << bitset<sizeof(int) * 8>(p_mask) << endl;
	cout << "s_mask:" << bitset<sizeof(int) * 8>(s_mask) << endl;
	getchar();
	SetProcessAffinityMask(h, 1);
	GetProcessAffinityMask(h, &p_mask, &s_mask);
	cout << hex << p_mask << endl;               //16 bit
	cout << "p_mask:" << bitset<sizeof(int) * 8>(p_mask) << endl;
	cout << "s_mask:" << bitset<sizeof(int) * 8>(s_mask) << endl;
	system("pause");
	return 0;
}