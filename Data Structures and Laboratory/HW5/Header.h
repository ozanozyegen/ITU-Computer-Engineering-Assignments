#ifndef STACK_H
#define STACK_H
struct record{
	char s_number[7]; //Student Number
	char name[20];
	char surname[20];
	double gpa;
	char department[3];

	record *next;
};
struct Stack{
	record *head;

	void create();
	void close();
	void push(record);
	record pop();
	double peak();
	int get_s_number();
	bool isempty();
};
#endif