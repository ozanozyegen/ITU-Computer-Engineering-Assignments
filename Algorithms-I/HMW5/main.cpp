#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>


/******************
 * BLG 335E Algorithms 1
 * Homework 5
 * Ozan Özyeğen
 * 150120115
 ******************/

using namespace std;
class node{
public:
    string name;
    int age;
    char gender;
    char color; //r for red, b for black
};
void read_data(node A[]); //Reads the file
void print_data(node A[]); //Prints the numbers

string f_name; //File name
int k=0;
int main(int argc, char **argv)
{
    f_name=argv[1];
    node *A=new node[100];
    read_data(A);
    print_data(A);

    delete[] A;
    return 0;
}
void read_data(node A[])
{
    const char *cf_name=f_name.c_str();

    ifstream fin(cf_name);
    if(fin.fail())
    {
        cerr<<"File could not be opened";
        cin.get();
        exit(1);
    }
    while(!fin.eof())
    {
        fin>>A[k].name>>A[k].gender>>A[k].age;
        k++;
    }
    cout<<"Data read\n";
    fin.close();
}
void print_data(node A[])
{
    int count=0;
    for (int i=0; i < k; i++)
    {
       cout << A[i].name<<" "<<A[i].gender<<" "<<A[i].age<<endl;
       count++;
    }
    cout<<"Count: "<<count<<endl<<endl;
}
