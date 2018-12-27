#include "Header.h"
#include <iostream>
#include <stdlib.h> //for atoi()
#include <string>

using namespace std;
void write_doc(Stack s);
int main(){
	Stack a, b;
	a.create(); //Temporary Stack
	b.create(); //Main Stack
	FILE *read;
	read = fopen("records.txt", "r");
	if (read == NULL){
		cout << "Error opening file" << endl;
		getchar();
		return 1;
	}
	while (!feof(read)){
		record temprecord;
		temprecord.next = NULL;
		fscanf(read, "%s %s %s %lf %s \n", &temprecord.s_number, &temprecord.name, &temprecord.surname, &temprecord.gpa, &temprecord.department);
		while (true)
		{
			if (b.isempty())
			{
				b.push(temprecord);
				break;
			}
			if (temprecord.gpa > b.peak()){
				b.push(temprecord);
				break;
			}
			else if (temprecord.gpa == b.peak()){
				if (atoi(temprecord.s_number) > b.get_s_number()){
					b.push(temprecord);
					break;
				}
				else{
					a.push(b.pop());
					b.push(temprecord);
					b.push(a.pop());
					break;
				}
			}
			if (!b.isempty()){
				a.push(b.pop());
			}
		}
		while (!a.isempty())
			b.push(a.pop());
	}
	write_doc(b);
	fclose(read);
	return 0;
}
void write_doc(Stack s){
	FILE *write;

	write = fopen("sortedRecords.txt", "w");
	record temprecord;
	while (!s.isempty()){
		temprecord = s.pop();
		string gpa = to_string(temprecord.gpa);
		fprintf(write, "%s ", temprecord.s_number);
		fprintf(write, "%s ", temprecord.name);
		fprintf(write, "%s ", temprecord.surname);
		fprintf(write, "%.2f ", temprecord.gpa);
		fprintf(write, "%s \n", temprecord.department);
	}

	fclose(write);


}
void Stack::create(){
	head = NULL;
}
void Stack::close(){
	record *p;
	while (head){
		p = head;
		head = head->next;
		delete p;
	}
}
void Stack::push(record newelement){
	record *new_record = new record;
	strcpy(new_record->department, newelement.department);
	strcpy(new_record->name, newelement.name);
	strcpy(new_record->surname, newelement.surname);
	strcpy(new_record->s_number, newelement.s_number);
	new_record->gpa = newelement.gpa;
	new_record->next = head;
	head = new_record;
}
record Stack::pop(){
	record *topnode;
	record temp;
	topnode = head;
	head = head->next;
	strcpy(temp.department, topnode->department);
	strcpy(temp.name, topnode->name);
	strcpy(temp.surname, topnode->surname);
	strcpy(temp.s_number, topnode->s_number);
	temp.gpa = topnode->gpa;
	return temp;
}
double Stack::peak(){
	if (head == NULL)
		return 0;
	return head->gpa;
}
int Stack::get_s_number(){
	return atoi(head->s_number);
}
bool Stack::isempty(){
	return (head == NULL);
}
