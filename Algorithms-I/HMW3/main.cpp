#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>

/******************
 * BLG 335E Algorithms 1
 * Homework 3
 * Ozan Özyeğen
 * 150120115
 ******************/

using namespace std;
class numbers{
public:
    int number;
};
void read_data(numbers A[]); //Reads the file
void print_data(numbers A[]); //Prints the numbers for test purposes
void quickSort(numbers A[],int first,int last);
int partition(numbers A[],int first,int last);
void countingSort(numbers A[]);
void radixSort(numbers A[]);
void countSort(numbers A[],int n,int exp);

char type; //Sort Type
string f_name; //File name
int arr_size=0; //Array Size
clock_t start; //Calculation time
int boundary;
int pivot;
int d;//Number of digits of max

int main(int argc, char **argv)
{
    type=*argv[1];
    f_name=argv[2];
    if(f_name=="1k-10k.txt"){
        d=5;
        arr_size=1000;
    }
    else if(f_name=="100k-10k.txt"){
        d=5;
        arr_size=100000;
    }
    else if(f_name=="1k-10M.txt"){
        arr_size=1000;
        d=8;
    }
    else if(f_name=="100k-10M.txt"){
        arr_size=100000;
        d=8;
    }
    else
    {
        cout<<"Input is wrong";
        return 0;
    }
    //Creating Array
    numbers *A = new numbers[arr_size];
    //cout << arr_size; //For test purposes

    //Reading the file and putting it into the array
    read_data(A);
    //print_data(A);
    //Starting calculation timer
    start=clock();
    long double duration;
    //Sort operations
    if(type=='q')
        quickSort(A,0,arr_size-1);
    else if(type=='c')
        countingSort(A);
    else if(type=='r')
        radixSort(A);
    else
        cout<<"Wrong sort type input";
    //print_data(A); //For test purposes
    duration = (clock() - start)/(double)CLOCKS_PER_SEC;
    cout<<"It took "<<duration<<" seconds.\n";

    delete[] A;
    return 0;
}

void read_data(numbers A[])
{
    const char *cf_name=f_name.c_str();
    ifstream fin(cf_name);
    if(fin.fail())
    {
        cerr<<"File could not be opened";
        cin.get();
        exit(1);
    }
    for(int i=0;i<arr_size;i++)
    {
        fin>>A[i].number;
    }
    cout<<"Data read\n";
    fin.close();
}
void print_data(numbers A[])
{
    int count=0;
    for (int i=0; i < arr_size; i++)
    {
       cout << A[i].number<<endl;
       count++;
    }
    cout<<"Count: "<<count<<endl<<endl;

}
void quickSort(numbers A[], int first, int last)
{
   if(first < last)
   {
       boundary = partition(A, first, last);
       quickSort(A, first, (boundary-1));
       quickSort(A, (boundary+1), last);
   }
}
int partition(numbers A[], int first, int last)
{
    int temp;
    pivot=A[last].number;
    boundary=first;
    for(int i=first;i<last;i++)
    {
        if(A[i].number<=pivot)
        {
            //Swap
            temp=A[i].number;
            A[i].number=A[boundary].number;
            A[boundary].number=temp;
            boundary++;
        }
    }
    //Swap
    temp=A[boundary].number;
    A[boundary].number=A[last].number;
    A[last].number=temp;
    return boundary;
}
void countingSort(numbers A[])
{
    int temp[arr_size];
    int max;
    for(int i=0;i<arr_size;i++)
        if(A[i].number>max)
            max=A[i].number;
    //The array that holds the countings of numbers
    int *counts = new int[max+1];
    for(int i=0;i<max+1;i++)
        counts[i]=0;
    for(int i=0;i<arr_size;i++)
        counts[A[i].number]=counts[A[i].number]+1;
    for(int i=2;i<max+1;i++)
        counts[i]=counts[i]+counts[i-1];
    for(int i=arr_size-1;i>=0;i--)
    {
        counts[A[i].number]=counts[A[i].number]-1;
        temp[counts[A[i].number]]=A[i].number;
    }
    for(int i=0;i<arr_size;i++)
        A[i].number=temp[i];

    delete[] counts;
}
void radixSort(numbers A[])
{
    //Find Max
    int max;
    for(int i=0;i<arr_size;i++)
        if(A[i].number>max)
            max=A[i].number;
    //For each digit
    for(int exp=1;max/exp>0;exp*=10)
        countSort(A,arr_size,exp);

}
void countSort(numbers A[], int n, int exp)
{
    //Output Array
    int output[n];
    int i, count[10]={0}; //Initialize 0

    for (i=0;i<n;i++)
        count[(A[i].number/exp)%10]++;

    for (i=1;i<10;i++)
        count[i]+=count[i - 1];
    // Organizing the output array
    for (i=n-1;i>=0;i--)
    {
        output[count[(A[i].number/exp)%10]-1]=A[i].number;
        count[(A[i].number/exp)%10]--;
    }
    //Copy the output array into the original array
    for (i=0;i<n;i++)
        A[i].number = output[i];
}
