/*
* File:   main.cpp
* Author: flamethrower
*
* Created on 26 Nisan 2014 Cumartesi, 11:13
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cstring>


using namespace std;

/*
*
*/


class Ship{
protected:
	char position_0;
	int position_1;
	char direction;
	int size;
public:
	Ship(char position_0_in, int position_1_in, char direction_in, int size_in) :position_0(position_0_in), position_1(position_1_in), direction(direction_in), size(size_in){}
	virtual bool damage_ship(char, int) = 0;
	virtual bool is_sank() = 0;
        virtual char get_type() = 0;
	int get_size(){ return size; }
	char get_position_0(){ return position_0; }
	int get_position_1(){ return position_1; }
	char get_direction(){ return direction; }
};

//Creating derived classes from abstract Ship class
class battleship :public Ship{
private:
	char coo_row[4]; //Row coordinates
	int coo_col[4]; //Column coordinates
public:
	battleship(char, int, char, int);
	bool damage_ship(char, int);
	bool is_sank();
        char get_type(){return 'B';};

};
battleship::battleship(char position_0_in, int position_1_in, char direction_in, int size_in) :Ship(position_0_in, position_1_in, direction_in, size_in){
    //cout << "Creating battleship\n" ;//Debug
	if (direction_in == 'h'){
		for (int i = 0; i < size; i++){
			coo_col[i] = position_1_in;
			coo_row[i] = position_0_in;
			position_1_in += 1;
		}
	}
	else if (direction_in == 'v'){
		for (int i = 0; i < size; i++){
			coo_col[i] = position_1_in;
			coo_row[i] = position_0_in;
			position_0_in += 1;
		}
	}
	else
	{
		cout << "Error"; //Debug
	}
    
    
    //cout << "Coordinates Created\n"; //Debugg
   /* for(int i=0;i<size;i++){//Debugg
        cout << coo_row[i] << coo_col[i] << endl;
    }*/
}
bool battleship::damage_ship(char position_0_in, int position_1_in){
    
    for (int i = 0; i < size; i++){
            if (position_0_in == coo_row[i]){
                    for (int j = 0; j < size; j++){
                            if (position_1_in == coo_col[i]){
                                    //The coordinates of a ship that has hit will be updated to z,1000. By checking the coordinates that is equal to z and 1000 , it is possible to determine how many parts of the ship got hit. 
                                    coo_row[i] = 'z';
                                    coo_col[i] = 1000;
                                    cout << " battleship is hit\n";
                                    return true; // attack_history update will be made in side's damage function
                            }
                    }

            }
    }
    //cout << " - " << position_0_in << position_1_in << ": missed\n";
    return false; //These coordinates are not on this ship.
}
bool battleship::is_sank(){
	int damaged = 0; //Number of parts that are damaged.
	for (int i = 0; i < size; i++){
		if (coo_row[i] == 'z' && coo_col[i] == 1000){
			damaged++;
		}
	}
	if (damaged == size)
		return true;
	else
		return false;
}

class cruiser : public Ship{
private:
    char coo_row[3]; //Row coordinates
    int coo_col[3]; //Column coordinates
public:
    cruiser(char, int, char, int);
    bool damage_ship(char, int);
    bool is_sank();
    char get_type(){return 'C';};
};
cruiser::cruiser(char position_0_in, int position_1_in, char direction_in, int size_in) :Ship(position_0_in, position_1_in, direction_in, size_in){
    if (direction_in == 'h'){
                for (int i = 0; i < size; i++){
                        coo_col[i] = position_1_in;
                        coo_row[i] = position_0_in;
                        position_1_in += 1;
                }
        }
        else if (direction_in == 'v'){
                for (int i = 0; i < size; i++){
                        coo_col[i] = position_1_in;
                        coo_row[i] = position_0_in;
                        position_0_in += 1;
                }
        }
        else
        {
                cout << "Error"; //Debug
        }
    
        //cout << "Coordinates Created\n"; //Debugg
        //for(int i=0;i<size;i++){//Debugg
          //      cout << coo_row[i] << coo_col[i] << endl;}
}
bool cruiser::damage_ship(char position_0_in, int position_1_in){
    	for (int i = 0; i < size; i++){
		if (position_0_in == coo_row[i]){
			for (int j = 0; j < size; j++){
				if (position_1_in == coo_col[i]){
					//The coordinates of a ship that has hit will be updated to z,1000. By checking the coordinates that is equal to z and 1000 , it is possible to determine how many parts of the ship got hit. 
					coo_row[i] = 'z';
					coo_col[i] = 1000;
					cout << " cruiser is hit\n";
					return true; // attack_history update will be made in side's damage function
				}
			}

		}
	}
	//cout << " - " << position_0_in << position_1_in << ": missed\n";
	return false; //These coordinates are not on this ship.
}
bool cruiser::is_sank(){
    	int damaged = 0; //Number of parts that are damaged.
	for (int i = 0; i < size; i++){
		if (coo_row[i] == 'z' && coo_col[i] == 1000){
			damaged++;
		}
	}
	if (damaged == size)
		return true;
	else
		return false;
}

class destroyer : public Ship{
private:
    char coo_row[2]; //Row coordinates
    int coo_col[2]; //Column coordinates
public:
    destroyer(char, int, char, int);
    bool damage_ship(char, int);
    bool is_sank();
    char get_type(){return 'D';};
};
destroyer::destroyer(char position_0_in, int position_1_in, char direction_in, int size_in) :Ship(position_0_in, position_1_in, direction_in, size_in){
    //cout << "Creating destroyer\n" ;//Debug
	if (direction_in == 'h'){
		for (int i = 0; i < size; i++){
			coo_col[i] = position_1_in;
			coo_row[i] = position_0_in;
			position_1_in += 1;
		}
	}
	else if (direction_in == 'v'){
		for (int i = 0; i < size; i++){
			coo_col[i] = position_1_in;
			coo_row[i] = position_0_in;
			position_0_in += 1;
		}
	}
	else
	{
		cout << "Error"; //Debug
	}
    
    
    //cout << "Coordinates Created\n"; //Debugg
    //for(int i=0;i<size;i++){//Debugg
      //  cout << coo_row[i] << coo_col[i] << endl;
    //}
}
bool destroyer::damage_ship(char position_0_in, int position_1_in){
    for (int i = 0; i < size; i++){
            if (position_0_in == coo_row[i]){
                    for (int j = 0; j < size; j++){
                            if (position_1_in == coo_col[i]){
                                    //The coordinates of a ship that has hit will be updated to z,1000. By checking the coordinates that is equal to z and 1000 , it is possible to determine how many parts of the ship got hit. 
                                    coo_row[i] = 'z';
                                    coo_col[i] = 1000;
                                    cout << " destroyer is hit\n";
                                    return true; // attack_history update will be made in side's damage function
                            }
                    }

            }
    }
    //cout << " - " << position_0_in << position_1_in << ": missed\n";
    return false; //These coordinates are not on this ship.
}
bool destroyer::is_sank(){
    int damaged = 0; //Number of parts that are damaged.
    for (int i = 0; i < size; i++){
            if (coo_row[i] == 'z' && coo_col[i] == 1000){
                    damaged++;
            }
    }
    if (damaged == size)
            return true;
    else
            return false;
}

class submarine : public Ship{
private:
    char coo_row[1]; //Row coordinates
    int coo_col[1]; //Column coordinates
public:
    submarine(char, int, char, int);
    bool damage_ship(char, int);
    bool is_sank();
    char get_type(){return 'S';};
};
submarine::submarine(char position_0_in, int position_1_in, char direction_in, int size_in) :Ship(position_0_in, position_1_in, direction_in, size_in){
    //cout << "Creating submarine\n" ;//Debug
    if (direction_in == 'h'){
            for (int i = 0; i < size; i++){
                    coo_col[i] = position_1_in;
                    coo_row[i] = position_0_in;
                    position_1_in += 1;
            }
    }
    else if (direction_in == 'v'){
            for (int i = 0; i < size; i++){
                    coo_col[i] = position_1_in;
                    coo_row[i] = position_0_in;
                    position_0_in += 1;
            }
    }
    else
    {
            cout << "Error"; //Debug
    }


/*cout << "Coordinates Created\n"; //Debugg
for(int i=0;i<size;i++){//Debugg
    cout << coo_row[i] << coo_col[i] << endl;
}*/
}
bool submarine::damage_ship(char position_0_in, int position_1_in){
    for (int i = 0; i < size; i++){
        if (position_0_in == coo_row[i]){
                for (int j = 0; j < size; j++){
                        if (position_1_in == coo_col[i]){
                                //The coordinates of a ship that has hit will be updated to z,1000. By checking the coordinates that is equal to z and 1000 , it is possible to determine how many parts of the ship got hit. 
                                coo_row[i] = 'z';
                                coo_col[i] = 1000;
                                cout << " submarine is hit\n";
                                return true; // attack_history update will be made in side's damage function
                        }
                }

        }
    }
    //cout << " - " << position_0_in << position_1_in << ": missed\n";
    return false; //These coordinates are not on this ship.
}
bool submarine::is_sank(){
    int damaged = 0; //Number of parts that are damaged.
for (int i = 0; i < size; i++){
        if (coo_row[i] == 'z' && coo_col[i] == 1000){
                damaged++;
        }
}
if (damaged == size)
        return true;
else
        return false;
}



class Side{
private:
	Ship **list;
	string attack_history[100];
	int counter;
public:
	Side(char*);
	void print() const;
	bool defeated() const;
	char damage(char, int);
	string attack();
	~Side(){ delete[] list; };

};
Side::Side(char* file_name){
	list = new Ship*[10];
	counter = 0;
	fstream in_file;
	char row;
	int col;
	char ship_type;
	char ship_direction;
	in_file.open(file_name);
	//cout << "Reading Ship Info\n"; //Debug
	while (in_file >> ship_type >> row >> col >> ship_direction)
	{
		//cout << ship_type << endl << row << col << endl << ship_direction << endl; // Debug
		if(ship_type == 'B'){
                    list[counter] = new battleship(row,col,ship_direction,4);// 4 is the size of the ship
                    counter++;
		}else if(ship_type == 'C'){
                    list[counter] = new cruiser(row,col,ship_direction,3);
                    counter++;
                }else if(ship_type == 'D'){
                    list[counter] = new destroyer(row,col,ship_direction,2);
                    counter++;
                }else if(ship_type == 'S'){
                    list[counter] = new submarine(row,col,ship_direction,1);
                    counter++;
                }
	}
       

	in_file.close();
}
void Side::print() const{
    char matrix[11][11];
     //Print preperation
    for(int i=0;i<11;i++)
        for(int j=0;j<11;j++)
            matrix[i][j] = ' ';
    for(int k=1;k<11;k++){
        matrix[0][k] = k-1+48;
        matrix[k][0] = 'a'+k-1; 
        //Putting ships into the matrix
        //matrix[list->get_position_0()-'a'+1][list->get_position_1()+1]=list[k]->get_type();
    }
    char direction;
    char position_0;
    int position_1;
    int size;
    char type;
    for(int i=0;i<10;i++){
        direction = list[i]->get_direction();
        position_0 = list[i]->get_position_0();
        position_1=list[i]->get_position_1();
        size = list[i]->get_size();
        type = list[i]->get_type();
        if (direction == 'h'){
                    for (int i = 0; i < size; i++){
                        matrix[position_0-'a'+1][position_1+1+i] = type;
                    }
            }
        else if (direction == 'v'){
                    for (int i = 0; i < size; i++){
                            matrix[position_0-'a'+1+i][position_1+1] = type;
                    }
            }
            else
            {
                    cout << "Error"; //Debug
            }
    }
    //Printing
    for(int i=0;i<11;i++){
        for(int j=0;j<11;j++){
            cout<<matrix[i][j]<<' ';
        }
        cout << "\n";
    }
}
string Side::attack(){
    bool unique;
    char a[3];
    while(!unique){
        char a1 = rand() %10 + 97;
        char a2 = rand() %10 + 48;
        a[0] = a1;
        a[1] = a2;
        a[2] = '\0';
        for(int i=0;i<counter;i++){
            if(attack_history[counter]==a){
                unique = false;break;
            }
        }unique =true;
    }
    return a;
}
char Side::damage(char p0, int p1){ //'0' == 48
    bool hit_check = false;
    for(int i=0;i<10;i++){
        if(list[i]->damage_ship(p0,p1)){
            char a[3];
            a[0] = p0;
            a[1] = p1+48;
            a[2] = '\0';
            //cout <<a <<" is b";//Debug
            attack_history[counter] = a;
            counter++;
            hit_check=true;
        }
    }
    if(hit_check==false){
    cout << " missed\n";
}
}
bool Side::defeated() const{
    int sank=0; // If all ships sank, then player defeated
    for(int i=0;i<10;i++){
        if(list[i]->is_sank()){
            sank++;
        }
        if(sank==10){
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv) {
	srand(time(NULL));
	Side player1("p1.bs");
        player1.print();
	Side player2("p2.bs");
        player2.print();
        while(true){
            string p1a = player1.attack();
            cout << "player1 - " << p1a << ": ";
            player2.damage(p1a[0],p1a[1]-48);
            if(player2.defeated()){
                cout << "That's a player1 win!";
                return 0;
            }
            string p2a = player2.attack();
            cout << "player2 - " << p2a << ": ";
            player1.damage(p2a[0],p2a[1]-48);
            if(player1.defeated()){
                cout << "That's a player2 win!";
                return 0;
            }
        }
         
        
	return 0;
}

