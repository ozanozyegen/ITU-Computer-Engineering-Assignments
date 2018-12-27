#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>


/******************
 * BLG 335E Algorithms 1
 * Homework 4
 * Ozan Özyeğen
 * 150120115
 ******************/

using namespace std;

class numbers{
public:
    int t;
};
int arr_size=2477;//Size of the hash table
int collusion=0;//Number of collusions
int count_delete=0;//Count of deletions from the table

void linear_probing(numbers A[],string command,string key);
void double_hashing(numbers A[],string command,string key);
void quadratic_hashing(numbers A[],string command,string key);
void delete_hash(numbers A[],int k);
void retrieve_hash(numbers A[],int k);
int main()
{
    cout << "Please select the hashing type\n"
         << "1-Linear Probing\n"
         << "2-Double Hashing\n"
         << "3-Quadratic Hashing\n"
         << "Enter the number of hashing type and press enter\n";
    int hash_type;
    cin>>hash_type;

    numbers *A=new numbers[arr_size]; //Creating the hashing table
    for(int i=0;i<arr_size;i++)
        A[i].t=0;
    string command="";//Retrieve,Insert,Delete
    string key="";//Hashing key

    ifstream fin("dataset.txt");
    if(fin.fail())
    {
        cerr<<"File could not be opened";
        cin.get();
        exit(1);
    }
    int count=0;
    while(!fin.eof())
    {
        string temp;
        //Reading
        fin>>temp;
        //Parsing
        char c;
        int i=0;
        command="";
        key="";
        while(temp[i]!=':')
        {
            c=temp[i];
            command+=c;
            i++;
        }
        i++;
        while(i!=temp.length())
        {
            c=temp[i];
            key+=c;
            i++;
        }
        if(hash_type==1)
            linear_probing(A,command,key);
        else if(hash_type==2)
            double_hashing(A,command,key);
        else if(hash_type==3)
            quadratic_hashing(A,command,key);

        count++;
    }

    cout<<"Data read\nCount="<<count<<endl;
    cout<<"Number of collusions="<<collusion<<endl;
    fin.close();
    delete[] A;

    return 0;
}
void linear_probing(numbers A[],string command,string key)
{
    int k=atoi(key.c_str());
    int i=0;
    if(command=="insert")
    {
        for(int m=0;m<arr_size;m++)
        {
            int h=(k+i)%arr_size;
            if(A[h].t==0||A[h].t==-1)
            {
                A[h].t=k;
                cout<<"Insertion: Key "<<k<<" is inserted in index:"<<h<<endl;
                break;
            }
            i++;
            collusion++;
        }

    }
    else if(command=="delete")
    {
        delete_hash(A,k);
    }
    else if(command=="retrieve")
    {
        retrieve_hash(A,k);
    }
}
void delete_hash(numbers A[],int k)
{
    bool found=false;
    for(int i=0;i<arr_size;i++)
    {
        if(A[i].t==k)
        {
            found=true;
            cout<<"Deletion: Key "<<k<<" is deleted from the table."<<endl;
            A[i].t=-1;//-1 means marked
            count_delete++;
            break;
        }
        if(count_delete==50)//Clear delete
        {
            for(int a=0;a<arr_size;a++)
            {
                if(A[a].t==-1)
                    A[a].t=0;
            }
            count_delete=0;
        }
    }
    if(found==false)
        cout<<"Deletion: Key "<<k<<" is not found in the table.\n";
}
void retrieve_hash(numbers A[],int k)
{
    bool found=false;
    for(int i=0;i<arr_size;i++)
    {
        if(A[i].t==k)
        {
            found=true;
            cout<<"Retrieve: Key "<<k<<" is found at index "<<i<<"."<<endl;
            break;
        }
    }
    if(found==false)
        cout<<"Retrieve: Key "<<k<<" is not found in the table.\n";
}
void double_hashing(numbers A[],string command,string key)
{
    int k=atoi(key.c_str());
    int i=0;
    if(command=="insert")
    {
        for(int m=0;m<arr_size;m++)
        {
            int h1=k%arr_size;
            int h2=1+k%(arr_size-4);
            int h=(h1+i*h2)%arr_size;
            if(A[h].t==0||A[h].t==-1)
            {
                A[h].t=k;
                cout<<"Insertion: Key "<<k<<" is inserted in index:"<<h<<endl;
                break;
            }
            i++;
            collusion++;
        }

    }
    else if(command=="delete")
    {
        delete_hash(A,k);
    }
    else if(command=="retrieve")
    {
        retrieve_hash(A,k);
    }
}
void quadratic_hashing(numbers A[],string command,string key)
{
    int k=atoi(key.c_str());
    int i=0;
    if(command=="insert")
    {
        for(int m=0;m<arr_size;m++)
        {
            int h1=k%arr_size;
            int h=(h1+i+2*i*i)%arr_size;
            if(A[h].t==0||A[h].t==-1)
            {
                A[h].t=k;
                cout<<"Insertion: Key "<<k<<" is inserted in index:"<<h<<endl;
                break;
            }
            i++;
            collusion++;
        }

    }
    else if(command=="delete")
    {
        delete_hash(A,k);
    }
    else if(command=="retrieve")
    {
        retrieve_hash(A,k);
    }
}
