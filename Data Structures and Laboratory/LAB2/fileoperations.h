#ifndef DOSYAISLEMLERI_H
#define DOSYAISLEMLERI_H
#include <stdio.h>
#include "record.h"

struct file{
	Phone_Record * phonebook;
	Phone_Record * temprecord;
	void create();
	void close();
	void printlist();
	void add(Phone_Record *);
	int search(char[]);
	void del(int recordno);
	void update(int recordno, Phone_Record *);
	int maxsize, numberofrecords;
};
#endif
