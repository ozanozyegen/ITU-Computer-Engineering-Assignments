#define _CRT_SECURE_NO_WARNINGS
#include "fileoperations.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

using namespace std;

void file::create(){
	numberofrecords = 0;
	maxsize = 8;
	phonebook = new Phone_Record[maxsize];
}

void file::close(){
	delete[] phonebook;
}

void file::printlist(){
	int counter = 0;
	for (int i = 0; i < numberofrecords; i++){
		cout << ++counter << ". " <<phonebook[i].name<< " " << phonebook[i].telno << endl;
	}
	getchar();
	getchar();
}

void file::add(Phone_Record * newrecord){
	if (numberofrecords == 0){
		phonebook[0].name = new char[strlen(newrecord->name)];
		phonebook[0].telno = new char[strlen(newrecord->telno)];

		strcpy(phonebook[0].name, newrecord->name);
		strcpy(phonebook[0].telno, newrecord->telno);

	}
	else {
		phonebook[numberofrecords].name = new char[strlen(newrecord->name)];
		phonebook[numberofrecords].telno = new char[strlen(newrecord->telno)];

		strcpy(phonebook[numberofrecords].name, newrecord->name);
		strcpy(phonebook[numberofrecords].telno, newrecord->telno);
	}
	numberofrecords++;

};
void file::del(int recordno) {
	for (int i = recordno-1; i < numberofrecords-2; i++){
		delete phonebook[i].name;
		delete phonebook[i].telno;
		phonebook[i].name = new char[strlen(phonebook[i + 1].name)+1];
		phonebook[i].telno = new char[strlen(phonebook[i + 1].telno)+1];
		strcpy(phonebook[i].name, phonebook[i + 1].name);
		strcpy(phonebook[i].telno, phonebook[i + 1].telno);

	}
	//delete phonebook[recordno].name;
	//delete phonebook[recordno].telno;
	numberofrecords--;

};