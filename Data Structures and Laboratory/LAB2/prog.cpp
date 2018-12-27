#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <ctype.h>
#include "fileoperations.h"
#include <string.h>
#include <stdio.h>
#include <cstring>

using namespace std;


typedef file Datastructure;
Datastructure list;

void print_menu();
bool operate(char choice);
void addrecord();
void removerecord();

bool cont = true;

int main(){
	list.create();
	while (cont){
		print_menu();
		char choice;
		cin >> choice;
		cont = operate(choice);
	}

	list.close();
	return EXIT_SUCCESS;
}

void print_menu(){
	system("cls");
	cout << endl << endl;
	cout << "Phone Records Application" << endl;
	cout << "Please choose one of the operations" << endl;
	cout << "R: Remove the Record" << endl;
	cout << "F: Search Record According to First Name" << endl;
	cout << "A: Add New Record" << endl;
	cout << "P: Print the Records" << endl;
	cout << "U: Update the Record" << endl;
	cout << "E: Exit" << endl;
	cout << endl;
	cout << "Enter your option {R, F, A, P, U ,E} : ";

}

bool operate(char choice){
	bool cont = true;
	switch (choice){
	case'A':case'a':{
				addrecord();
				break;
	}
	case'L':case'l':{
				list.printlist();
				break;
	}
	case'R':case'r': removerecord();
		break;
	case'E':case'e':{
				cout << "Are you sure? (Y/N): ";
				cin >> choice;
				if (choice == 'Y' || choice == 'y'){
					cont = false;
					cout << "Exitting..." << endl;
					getchar();
				}
				break;
	}
	default:{
				cout << "Invalid choice. Try again." << endl;
				getchar();
				break;
	}
	}
	return cont;
}

void addrecord(){
	Phone_Record * newrecord;
	newrecord = new Phone_Record;
	char tempname[30];
	cout << "Enter new information." << endl;
	cout << "Name and Surname: ";
	cin.ignore(1000, '\n');
	cin.getline(tempname, 30);
	newrecord->name = new char[strlen(tempname)];
	newrecord->name = tempname;

	char temptelno[30];
	cout << "Telephone number: ";
	cin.getline(temptelno, 30);
	newrecord->telno = new char[strlen(temptelno)];
	newrecord->telno = temptelno;

	list.add(newrecord);

}
void removerecord() {
	int indexnum;
	cout << " Please enter the index number of the record" << endl;
	cout << "Index No: ";
	cin >> indexnum;
	list.del(indexnum);

	getchar();
	getchar();
}