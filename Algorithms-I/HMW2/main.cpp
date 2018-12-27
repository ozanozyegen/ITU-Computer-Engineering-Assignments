#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;
class numbers{
public:
    int number;
};


void read_data(numbers A[], int lineNo); //Reads the file
void write_data(numbers A[],int best,long double duration);

clock_t start;

int main(int argc, char **argv)
{
    int lineNo=atoi(argv[1]); //STARTS FROM 1 !!!
    int k=atoi(argv[2]);

    numbers A[10];
    read_data(A,lineNo);

    start=clock();
    long double duration;

    int i=0,best;
    int referance=0;//Score of the best person in the first k number of people
    //Reject k people
    while(k>0){
        if(A[i].number>A[referance].number)
            referance=i;
        i++;
        k--;
    }
    best=i;
    while(i<10){
        if(A[i].number>A[referance].number){
            best=i;
            break;
        }else if(i==9){
            best=A[i].number;
        }
        i++;
    }

    duration = (clock() - start)/(double)CLOCKS_PER_SEC;
    //cout<<"It took me "<<duration<<" seconds.";
    write_data(A,best,duration);
    /*
    for(int j=0;j<10;j++)
        cout<<A[j].number<<" ";
    */
    return 0;
}
void read_data(numbers A[],int lineNo)
{
    ifstream fin;
    fin.open("data.txt", ios::in);
    if (fin.fail())
        {
            cerr << "File could not be opened\n";
            cin.get();
            exit( 1 );
        }
    char ch;
    int number_of_lines = 1;
    int i=0;
    while (!fin.eof() ) {
        while(number_of_lines!=lineNo&&!fin.eof())
        {
            fin.get(ch);
            if(ch=='\n')
                number_of_lines++;
        }
        fin.get(ch);
        //cout << ch;
        if (ch == '\n')
            number_of_lines++;
        else if(ch == '1'){
            fin.get(ch);
            if(ch == '0')
                A[i++].number=10;
            else
                A[i++].number=1;
        }
        else if(ch != ' ' && ch != ',')
            A[i++].number=ch-'0';
    }

    //cout << "data read\n";

    fin.close();
}
void write_data(numbers A[],int best,long double duration)
{
    ofstream fout;
    fout.open( "best_applicant.txt", ios::out);
    fout << "Applicant Index: " << best << endl;
    fout << "Applicant Score: " << A[best].number << endl;
    fout<<"It took "<<duration<<" seconds.";
    fout.close();
}
