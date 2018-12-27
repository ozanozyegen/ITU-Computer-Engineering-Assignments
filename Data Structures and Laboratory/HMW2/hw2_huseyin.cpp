/*
* @Chousein Giousouf
* Student Name: Chousein Giousouf
* Student ID : 150120913
* Date: 10.10.2014
*/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <iomanip>
#include <ctype.h>

using namespace std;

struct rna
{
	char ele;
	rna *next;  //her nodun sonrakisini tutan pointer
	
}; //these rna elements will be used as heads

rna *rna1;  //head pointers of  RNA chains !!
rna *rna2;
rna *rna3;
rna *rna4;
rna *rna5;

void initrna(char arr[],rna*headptr)  //initialise RNA chains and print them!!
{   

	rna *traverse; //pointer to traverse
	//headptr = new rna; //get memory for it
	//headptr->ele = arr[0]; //headptr is the head of the given rna 
	//headptr->next = NULL;
	
	traverse = headptr;  //travers u ilk noda esitle
	//traverse = traverse->next;
	for (int j = 1; j < 10; j++)
	{
		rna *newnode;   //create a new node
		newnode = new rna; //get memory for it
		newnode->ele= arr[j];
		newnode->next = NULL;//make the next part NULL
	
		traverse->next = newnode;  //newnode u travers in sonuna ekle
		cout << traverse->ele;
		traverse = traverse->next;
	
	}
	cout << traverse->ele;
	cout << endl;
	
}

void coupleDNA(rna *ptr1, rna *ptr2, rna *ptr3, rna *ptr4, rna *ptr5)
{
	char arr[] = "ATGC"; // array to compare A / T / G / C   nucleic acids
	rna *tra1; //traverse pointer for every RNA chain
	rna *tra2; //traverse pointer for every RNA chain
	

	int counter = 0; // counter to count matched nucleic acids;
	tra1 = ptr1;
	tra2 = ptr2;
	
	rna* ptrarr[5] = { rna1, rna2, rna3, rna4, rna5 };  //create an array of pointers !!
	char DNA1[11];  //array to keep first nucleic acid of DNA !
	char DNA2[11]; //array to keep second nucleic acid of DNA !
	bool cikis = 0;  // fonksiyonu bitirmek icin gereklilik sarti
	int i = 0;
	int j = 0;
	//compare rna1 with others..
	while (!cikis)
	{
		// if statement controls the maching of  A/T and G/C
		if ((tra1->ele == arr[0] && tra2->ele == arr[1]) || (tra1->ele == arr[1] && (tra2->ele == arr[0])) || (tra1->ele == arr[2] && tra2->ele == arr[3])|| (tra1->ele == arr[3] && tra2->ele == arr[2]) )
			{
				DNA1[counter] = tra1->ele;
				DNA2[counter] = tra2->ele;
				tra1 = tra1->next;  //traverse 1 i ilerlet
				tra2 = tra2->next;  // traverse 2 yi ilerlet
				counter++;
				if (counter == 10)
				{
				
					cikis = true;
				}
			}
			else{
				i++;
				if (i == j) i++;  // ayni RNA lar icin control yapma
				counter = 0;
				tra1 = ptrarr[j];  //for loop la eslesstir bul bi yolunu
				tra2 = ptrarr[i];
				if (i == 4){
					i = 0;
					j++;
				}
		}
		
	}
	cout << "DNA : " << endl << endl;;
	for (int i = 0; i < 10; i++)
	{
		cout << DNA1[i] << DNA2[i] << "  ";
	}
}


int main(){
	char rna1_elements[] = "ATGATGATGC";//initialise 1. RNA
	cout << "RNA1 : ";
	rna1 = new rna;   //get memory for pointer
	rna1->ele =rna1_elements[0];//initialise the first RNA
	rna1->next = NULL; //initialise next to NULL
	initrna(rna1_elements,rna1); //arry i alip rna1 icin linked list yapar
	cout << endl;
	char rna2_elements[] = "TCGCGCTAGC"; //initialise 2. RNA
	cout << "RNA2 : ";
	rna2 = new rna;   //get memory for pointer
	rna2->ele = rna2_elements[0];//initialise the first element
	rna2->next = NULL;//initialise next to NULL
	initrna(rna2_elements,rna2);
	cout << endl;
	char rna3_elements[] = "CGTCGTAAAC";//initialise 3. RNA
	cout << "RNA3 : ";
	rna3 = new rna;   //get memory for pointer
	rna3->ele = rna3_elements[0];//initialise the first element
	rna3->next = NULL;//initialise next to NULL
	initrna(rna3_elements, rna3);
	cout << endl;
	char rna4_elements[] = "TATTTACGAA";//initialise 4. RNA
	cout << "RNA4 : ";
	rna4 = new rna;   //get memory for pointer
	rna4->ele = rna4_elements[0];//initialise the first element
	rna4->next = NULL;//initialise next to NULL
	initrna(rna4_elements, rna4);
	cout << endl;
	char rna5_elements[] = "TACTACTACG";//initialise 5. RNA
	cout << "RNA5 : ";
	rna5 = new rna;   //get memory for pointer
	rna5->ele = rna5_elements[0];//initialise the first element
	rna5->next = NULL;//initialise next to NULL
	initrna(rna5_elements, rna5);
	cout << endl;
	coupleDNA(rna1, rna2, rna3, rna4, rna5);
	cout << endl<<endl;
	system("Pause");
	return 0;
}
