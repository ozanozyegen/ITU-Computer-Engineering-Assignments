#ifndef DOSYAISLEMLERI_H
#define DOSYAISLEMLERI_H
#include <stdio.h>
#include "kayit.h"
struct Document{
	char *doc_name;
	FILE *note_file;
	void remove(int num);
        int search_department(char[]);
	int search_name(char[]);
	void add(faculty_record *);
        int print();
	void update(int num,faculty_record *);
        void maintenance(int num,faculty_record *);
        
        void create();
        int fix_records();
	void close();
};
#endif
