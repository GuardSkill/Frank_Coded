#include <iostream>
using namespace std;
typedef char* cstring;
int length(cstring x)
{
	int i = 0;
	while (x[i] != 0)
	{
		i++;
	}
	return i;
}
void concatenate(cstring& a, cstring b)
{
	int old_length = length(a);
	int b_length = length(b);
	cstring newstr = new char[old_length + b_length + 1];
	int i = 0;
	while (i<old_length)
	{
		newstr[i] = a[i];
		i++;
	}
	i = 0;
	while (i < b_length + 1)
	{
		newstr[old_length + i] = b[i];
		i++;
	}
	delete[3] a;
	a = newstr;

}
void main()
{
  /*char* x = "abc";
	cout<<strlen(x)<<endl;  
	x[1] = 'c';                �޷��ı����ĳ���
	cout << x;*/
	cstring a = new char[3];
	//a = "ab";  �����⽫aָ����һ���ڶ���ı�����ԭ��New���ڴ�ֱ��dangle
	a[0] ='a' ; a[1] = 'b'; a[2] = 0;
	cstring b = new char[3];
	b = "34";
	concatenate(a, b);
	cout << a;
}
