#include <iostream>

using namespace std;
struct student
{
    int id;
    char   name[20];
    double price;
};
static int compare_Price(const void *void_a,const void *void_b)
{
    student *a=(student  *)void_a;
    student *b=(student *)void_b;
    return a->price-b->price;
}
static int compare_ID(const void *void_a,const void *void_b)
{
    student *a=(student  *)void_a;
    student *b=(student *)void_b;
    return a->id-b->id;
}
int main()
{
    struct student st[3]={{1,"lisiyuan",99},{2,"zhj",90},{3,"xsp",100}};
    qsort(st,3,sizeof(student),compare_Price);
    int i=0;
    while(i<3)
    {
        cout<<i+1<<":"<<st[i].name<<endl;
        i++;
    }
   i=0;
    qsort(st,3,sizeof(student),compare_ID);
    while(i<3)
    {
        cout<<i+1<<":"<<st[i].name<<endl;
        i++;
    }
   return 0;
}
