#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <ctype.h>
#include "dosyaislemleri.h"
using namespace std;
typedef Document Datastructure;


Datastructure notebook;
void fix_record(){ //Deletes the empty record lines
	int emptyrecord = notebook.fix_records();
	if (emptyrecord == 0){
		cout << "File has been fixed." << endl;
	}
else
	cout << "File has been fixed, empty records are cleared." <<endl;
	getchar();

};
void write_menu();
bool process(char);
void search_name();
void search_department();
void add_record();
void remove_record();
void update_record();
void print_record();

int main(){
        notebook.create();
	bool finish = false;
	char selection;
	while (!finish) {
		write_menu();
		cin >> selection;
		finish = process(selection);
        }
        notebook.close();
	return EXIT_SUCCESS;
}
void write_menu(){
	system("clear");
	cout << endl << endl;
	cout << "Faculty Record Application" << endl;
	cout << "Choose an option" << endl;
        cout << "R: Remove Record" << endl;
	cout << "S: Search Records by Department" << endl;
        cout << "F: Search Records by First Name" << endl;
	cout << "A: Add Record" << endl;
        cout << "P: Print all records" << endl;
	cout << "U: Update Record" << endl; //Program will ask which part is going to be updated
	cout << "E: Exit" << endl;
	cout << endl;
	cout << "Enter your option {R, S, F, A, P, U, E} : ";
}
bool process(char selection){
	bool end = false;
	switch (selection) {
	case 'S': case 's': //Search records by department
		search_department();
		break;
        case 'F': case 'f': //Search records by name
		search_name();
		break;
	case 'A': case 'a':
		add_record();
		break;
        case 'P': case 'p': //Print all records
		print_record();
		break;
	case 'U': case 'u':
		update_record();
		break;
	case 'R': case 'r': //Remove record
		remove_record();
		break;
	case 'E': case 'e':
		cout << "Are you sure that you want to terminate the program? (E/H):";
		cin >> selection;
		if (selection == 'E' || selection == 'e')
			end = true;
		break;
	default:
		cout << "Error: You have made an invalid choice" << endl;
		cout << "Try again {R, S, F, A, P, U, E} :";
		cin >> selection;
		end = process(selection);
		break;
	}
	return end;

}
void search_name(){
	char fname[L_FIRSTNAME];
	cout << "Please enter the name of the person you want to search (press '*' for listing all):" << endl;
	cin.ignore(1000, '\n');
	cin.getline(fname,L_FIRSTNAME); 
        if (notebook.search_name(fname) == 0){
		cout << "Record can not be found" << endl;
	}
	getchar();
}
void search_department(){
    	char dept_n[L_DEPARTMENT];
	cout << "Please enter the department of the people you want to search (press '*' for listing all):" << endl;
	cin.ignore(1000, '\n');
	cin.getline(dept_n,L_DEPARTMENT); 
        if (notebook.search_department(dept_n) == 0){
		cout << "Record can not be found" << endl;
	}
	getchar();
}
void add_record(){
	faculty_record new_record;
	cout << "Please enter the information of the person you want to save " << endl;
	cout << "First Name : ";
        cin.ignore(1000, '\n');
	cin.getline(new_record.fname, L_FIRSTNAME);
 	cout << "Last Name : ";
	//cin.ignore(1000, '\n');
	cin.getline(new_record.lname, L_LASTNAME);
        cout << "Department : ";
	//cin.ignore(1000, '\n');
	cin.getline(new_record.dept, L_DEPARTMENT);
	cout << "Phone number :";
	cin >> setw(L_TELNO) >> new_record.phone;
        cout << "Office number :";
	cin >> setw(L_TELNO) >> new_record.office;
	notebook.add(&new_record);
	cout << "Record has been added" << endl;
	getchar();
}
void remove_record(){
	char name[L_FIRSTNAME];
	int selection;
	cout << "Please enter the name of the person you want to delete (press '*' for listing all):" << endl;
	cin.ignore(1000, '\n');
	cin.getline(name,L_FIRSTNAME);
	int num_people = notebook.search_name(name);
	if (num_people == 0){
		cout << "Record can not be found" << endl;
	}
	else {
		if (num_people == 1){
			cout << "Record has been found." << endl;
			cout << "Please enter the index of the record if you want to delete this contact (Press -1 to exit without deletion): ";
		}
		else
			cout << "Please enter the index of the record that you want to delete (Press -1 to exit without deletion): ";
		cin >> selection;
		if (selection == -1) return;
		notebook.remove(selection);
		cout << "Record has been deleted" << endl;
	}
	fix_record();
	getchar();
}
void update_record(){
	char name[L_FIRSTNAME];
	int selection;
        char yesorno;
	cout << "Please enter the name of the person you want to update (press '*' for listing all):";
	cin.ignore(1000, '\n');
	cin.getline(name,L_FIRSTNAME);
	int numberofpeople = notebook.search_name(name);
	if (numberofpeople == 0){
		cout << "Record can not be found" << endl;
	}
	else {
		if (numberofpeople == 1){

			cout << "Record has been found." << endl;
			cout << "Please enter the index of the record if you want to update this contact (Press -1 to exit without updating) ";
		}
		else
			cout << "Please enter the index of the record that you want to update (Press -1 to exit without updating): ";
		cin >> selection;
		if (selection == -1) return;
                faculty_record new_record;
                notebook.maintenance(selection, &new_record);
                
		cout << "Do you want to change the first name? (y/n)" << endl;
                cin >> yesorno;
                if(yesorno == 'y' || yesorno == 'Y'){
		cout << "Please enter the new first name : ";
		cin.ignore(1000, '\n');
		cin.getline(new_record.fname,L_FIRSTNAME);
                }
                cout << "Do you want to change the last name? (y/n)" << endl;
                cin >> yesorno;
                if(yesorno == 'y' || yesorno == 'Y'){
		cout << "Please enter the new last name : ";
                cin.ignore(1000, '\n');
		cin.getline(new_record.lname,L_LASTNAME);
                }
                cout << "Do you want to change department? (y/n)" << endl;
                cin >> yesorno;
                if(yesorno == 'y' || yesorno == 'Y'){
		cout << "Please enter the new department name : ";
                cin.ignore(1000, '\n');
		cin.getline(new_record.dept,L_DEPARTMENT);
                }
                cout << "Do you want to change the Phone number? (y/n)" << endl;
                cin >> yesorno;
                if(yesorno == 'y' || yesorno == 'Y'){
		cout << "Please enter the new phone number : ";
                cin.ignore(1000, '\n');
                cin >> setw(L_TELNO) >> new_record.phone;
                }
                cout << "Do you want to change the Office number? (y/n)" << endl;
                cin >> yesorno;
                if(yesorno == 'y' || yesorno == 'Y'){
		cout << "Please enter the new office number : ";
                cin.ignore(1000, '\n');
                cin >> setw(L_TELNO) >> new_record.office;
                }
                
		notebook.update(selection, &new_record);
		cout << "Record has been updated successfully" << endl;
	}
	getchar();
}
void print_record(){
    if(notebook.print()==0)
        cout << "No records found.";
    getchar();
    getchar();
}
