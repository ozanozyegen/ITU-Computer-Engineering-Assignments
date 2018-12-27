#include "dosyaislemleri.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;

void Document::add(faculty_record *ykptr){
	fseek(note_file, 0, SEEK_END);
	fwrite(ykptr, sizeof(faculty_record), 1, note_file);
}
void Document::create(){
	doc_name = "faculty_records.txt";
	note_file = fopen(doc_name, "r+");
	if (!note_file){
		if (!(note_file = fopen(doc_name, "w+"))){
			cerr << "File can not be opened" << endl;
			exit(1);
		}
	}
}
void Document::close(){
	fclose(note_file);
}
int Document::search_name(char name[]){
	faculty_record k;
	int counter = 0;
	bool all = false;
	int found = 0;
	if (strcmp(name, "*") == 0)
		all = true;
	fseek(note_file, 0, SEEK_SET);
	while (!feof(note_file)){
		counter++;
		fread(&k, sizeof(faculty_record), 1, note_file);
		if (feof(note_file)) break;
		if (!all && strncasecmp(k.fname, name, strlen(name)) != 0)
			continue;
		cout << counter << "." << k.fname << " " << k.lname << " " << k.dept << " " << k.phone << " " << k.office << endl;
		found++;
	}
	return found;
}
int Document::search_department(char d_name[]){
	faculty_record k;
	int counter = 0;
	bool all = false;
	int found = 0;
	if (strcmp(d_name, "*") == 0)
		all = true;
	fseek(note_file, 0, SEEK_SET);
	while (!feof(note_file)){
		counter++;
		fread(&k, sizeof(faculty_record), 1, note_file);
		if (feof(note_file)) break;
		if (!all && strncasecmp(k.dept, d_name, strlen(d_name)) != 0)
			continue;
		cout << counter << "." << k.fname << " " << k.lname << " " << k.dept << " " << k.phone << " " << k.office <<  endl;
		found++;
	}
	return found;
}
void Document::update(int num,faculty_record *ykptr){
    if(fseek(note_file, sizeof(faculty_record)*(num - 1), SEEK_SET) == 0)
        fwrite(ykptr, sizeof(faculty_record), 1, note_file);
}

void Document::maintenance(int num,faculty_record *ykptr){
    faculty_record k;
    if(fseek(note_file, sizeof(faculty_record)*(num - 1), SEEK_SET) == 0)
        fread(&k, sizeof(faculty_record), 1, note_file);
    strcpy(ykptr->fname,k.fname);
    strcpy(ykptr->lname,k.lname);
    strcpy(ykptr->dept,k.dept);
    ykptr->phone=k.phone;
    ykptr->office=k.office;
}

void Document::remove(int num){
	faculty_record emptyrecord = {"", ""};
	if (fseek(note_file, sizeof(faculty_record)*(num - 1), SEEK_SET) == 0)
		fwrite(&emptyrecord, sizeof(faculty_record), 1, note_file);
}
int Document::fix_records(){
	int counter = 0;
	FILE* n_notefile = NULL;
	n_notefile = fopen("faculty_records2.txt", "w+");
	if (!n_notefile){
		cerr << "Second File can not be opened" << endl;
		exit(1);
	}
	faculty_record record;
	fseek(note_file, 0, SEEK_SET);
	while (!feof(note_file)){
		fread(&record, sizeof(faculty_record), 1, note_file);
		if (feof(note_file)) break;
		if (strcmp(record.fname,"") == 0) 
                    counter++;
		else
		fwrite(&record, sizeof(faculty_record), 1, n_notefile);
	}
	if (counter > 0){
		fclose(note_file);
		fclose(n_notefile);
		char com[500] = "cp ";
		strcat(com,"faculty_records2.txt");
		strcat(com, " ");
		strcat(com, "faculty_records.txt");
		system(com);
		create();
		return counter;
	}


}
int Document::print(){
    faculty_record k;
    int counter = 0, found = 0;
    fseek(note_file, 0, SEEK_SET);
    while (!feof(note_file)){
            counter++;
            fread(&k, sizeof(faculty_record), 1, note_file);
            if(feof(note_file)) break;
            cout << counter << "." << k.fname << " " << k.lname << " " << k.dept << " " << k.phone << " " << k.office << endl;
            found++;
        }
    return found;
}
