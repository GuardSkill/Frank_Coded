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
	x[1] = 'c';                无法改变堆里的常量
	cout << x;*/
	cstring a = new char[3];
	//a = "ab";  错误，这将a指向了一个在堆里的变量，原来New的内存直接dangle
	a[0] ='a' ; a[1] = 'b'; a[2] = 0;
	cstring b = new char[3];
	b = "34";
	concatenate(a, b);
	cout << a;
}
