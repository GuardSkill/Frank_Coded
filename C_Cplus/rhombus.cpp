#include <iostream>
#include <assert.h>

using namespace std;
bool cpystr(char *a,char*b)
{
    assert(a!=NULL);
    assert(b!=NULL);
    while((*a++=*b++)!='\0');
    return true;
}
int rhombus()
{
   int i,j;
   for(i=0;i<9;i++)
   {
       for(j=0;j<5+4-abs(4-i);j++)
       {
           if(j<abs(4-i)) cout<<" ";
           else cout<<"#";
       }
       cout<<"\n";
   }
    return 0;
}
