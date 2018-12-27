#include <iostream>
#include <fstream>
#include<stdlib.h>
#include <time.h>

using namespace std;
class numbers{
public:
    int number;
};
void read_data(numbers A[]); //Reads the file
void print_data(numbers A[]); //Prints the numbers, for test purposes
void bubble_sort(numbers A[],int size);
void merge_sort(numbers A[], int size);
void merge_arrays(numbers outputArray[],numbers firstArray[], int size1, numbers secondArray[],int size2);



char type;
int size;

clock_t start;

int main(int argc, char **argv)
{

    type=*argv[1];//b for Bubble or m for Merge
    size=atoi(argv[2]);//1000 10000 100000 1000000
    //cout<<"Size: "<<size<<endl;
    numbers A[size];//Creating the array of size given
    //A=new int[size];
    //for(int a=0;a<size;a++)
        //A[a]=0;


    read_data(A);
    //print_data(A); //For test purposes
    //Counting time spent on CPU
    start=clock();
    long double duration;

    if (*argv[1]=='b')
        bubble_sort(A,size);
    else if (*argv[1]=='m')
        merge_sort(A,size);
    else
        cout << "Invalid argument! b or m as first argument and size as second argument accepted";

    duration = (clock() - start)/(double)CLOCKS_PER_SEC;
    //print_data(A); //For test purposes
    cout<<"It took me "<<duration<<" seconds.";



    //delete[] A;
    return 0;
}

void read_data(numbers A[])
{

    ifstream fin;
    fin.open("data.txt", ios::in);
    if (fin.fail())
        {
            cerr << "File could not be opened\n";
            cin.get();
            exit( 1 );
        }
    for(int i=0;i<size;i++)
    {
        fin >> A[i].number;
    }
    cout << "data read\n";
    fin.close();
}
void print_data(numbers A[])
{
    int count=0;
    ofstream fout;
    fout.open( "sorted.txt", ios::out);

    for (int i=0; i < size; i++)
   {
       fout << A[i].number<<endl;
       cout << A[i].number<<endl;
       count++;
   }
    cout<<"Count: "<<count<<endl<<endl;
    fout.close();  // ofstream destructor closes file
}
void bubble_sort(numbers A[], int size)
{
    int temp;

    for(int i=0;i+1<size;i++)
        for(int j=0;j+1<size;j++)
        {
            if(A[j].number>A[j+1].number)
            {
                temp=A[j].number;
                A[j].number=A[j+1].number;
                A[j+1].number=temp;
            }
        }
}
void merge_sort(numbers A[], int size)
{
    if (size==1)
        return;
    int mid=size/2;
    int low=mid;
    int high=size-mid;

    numbers firstarray[low];
    numbers secondarray[high];

    for(int i=0;i<size;i++)
    {
        if(i<mid)
            firstarray[i].number=A[i].number;
        else
            secondarray[i-mid].number=A[i].number;
    }
    merge_sort(firstarray, low);
    merge_sort(secondarray, high);
    merge_arrays(A, firstarray,low,secondarray,high);
}
void merge_arrays(numbers outputArray[],numbers firstArray[], int size1, numbers secondArray[],int size2)
{
    int p=0;
    int p1=0;
    int p2=0;
    while(p1<size1&&p2<size2)
    {
        if(firstArray[p1].number<=secondArray[p2].number)
            outputArray[p++].number=firstArray[p1++].number;
        else
            outputArray[p++].number=secondArray[p2++].number;
    }

    while(p1<size1) outputArray[p++].number=firstArray[p1++].number;

    while(p2<size2) outputArray[p++].number=secondArray[p2++].number;
}
