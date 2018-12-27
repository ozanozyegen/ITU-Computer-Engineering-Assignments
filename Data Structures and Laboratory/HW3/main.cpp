/* 
 * File:   main.cpp
 * Author: Ozan Özyeğen
 *
 * Created on October 28, 2014, 4:49 PM
 */

#include "liste.h"
#include <string.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
card *thead; //TopList Head
card *ttail;
board *list; //Head of Board List
board *tlist; //Traverse of Board List
board *flist; //Head of Foundation List
board *tflist; //Traverse of Foundation List
char arrvalue[13] = {'K','Q','J','1','9','8','7','6','5','4','3','2','A'};

int value(card *a){ //How valuable is card a K=13 , A=1 etc..
    int val = 0;
    while(a->number[0]!=arrvalue[val]){
        val++;
    }
    return 13-val;
    
}

void resetlist(board* blist){
    //After this function all tcards of the board shows the head
    //This function is written since we need to do this many times in the program
    //Only put TLIST into this function
    while(blist){
        blist->tcard = blist->hcard;
        blist = blist->next;
    }
}

int maxnumberofcards(board* maxlist){
    //This function returns the maximum amount of cards in a board type multi list
    int maxcount = 0;
    int count = 0;
    board *head = maxlist;
    while(maxlist){
        count = 0;
        maxlist->tcard = maxlist->hcard;
        while(maxlist->tcard){
            maxlist->tcard = maxlist->tcard->next;
            count ++;
        }
        if(count > maxcount)
            maxcount = count;
        maxlist = maxlist->next;
    }
    resetlist(head);
    return maxcount;
}

void printboard(){ 
    //A list and a traverse of that board list is needed for the operation
    //Printing Board List 123..7
    cout << "Board Lists: \n";
    tlist = list; //Going head
    cout << "1. list\t2. list\t3. list\t4 .list\t5. list\t6. list\t7. list\n";
    //Ready to print board list
    int maxcards=maxnumberofcards(list);
    for(int i=0;i<maxcards;i++){
        tlist = list; //tlist shows head
        while(tlist){
            if(tlist->tcard == NULL){
                cout << "       \t";
            }
            else if(strcmp(tlist->tcard->updown,"Down")==0){
                cout << "X      \t";
            }else if(strcmp(tlist->tcard->updown,"Up")==0){
                cout << tlist->tcard->color << "," << tlist->tcard->suit << "," << tlist->tcard->number << "\t";
            }
            if(tlist->tcard)
                tlist->tcard = tlist->tcard->next;
            if(tlist)
                tlist = tlist->next;
        }
        cout << endl;
    }
}

void createfoundationlist(){
    //Creating Foundation List
    flist = new board; //Head of the Foundation list
    flist->next = NULL;
    flist->hcard =NULL;
    flist->tcard = NULL;
    tflist = flist; //Traverse(Temporary) Foundation list pointer
    for(int i=0;i<3;i++){
        board *tboard = new board; //Temporary Board for the creation of the Foundation List
        tboard->next = NULL;
        tboard->hcard = NULL;
        tboard->tcard = NULL;
        tflist->next = tboard;
        tflist = tflist->next;
    }
    //Foundation List Created
}

void printfoundationlist(){
    resetlist(flist);
    tflist = flist;
    cout << "Foundation Lists:\n\n";
    cout << "Spades\tHearts\tDiamonds\tClubs\n";
    //Ready to print board list
    int maxcards=maxnumberofcards(flist);
    int count = 0; //Count is for understanding how much space needed when writing the Foundation list
    for(int i=0;i<maxcards;i++){ 
        tflist = flist; //tlist shows head
        count=0;//When it is in the head of the linked list counter is 0
        while(tflist){
            if(tflist->tcard == NULL){
                if(count==0||count==1) //Spades and Hearts
                    cout << "      \t";
                else if(count==2) //Diamonds
                    cout << "        \t";
                else if(count==3) //Clubs
                    cout<< "     \n";
            }
            else if(tflist->tcard->suit=='S'){
                cout << tflist->tcard->color << "," << tflist->tcard->suit << "," << tflist->tcard->number << " \t";
            }
            else if(tflist->tcard->suit=='H'){
                cout << tflist->tcard->color << "," << tflist->tcard->suit << "," << tflist->tcard->number << " \t";
            }
            else if(tflist->tcard->suit=='D'){
                cout << tflist->tcard->color << "," << tflist->tcard->suit << "," << tflist->tcard->number << "   \t";
            }
            else if(tflist->tcard->suit=='C'){
                cout << tflist->tcard->color << "," << tflist->tcard->suit << "," << tflist->tcard->number << "\n";
            }
            count++;
            if(tflist->tcard)
                tflist->tcard = tflist->tcard->next;
            if(tflist)
                tflist = tflist->next;
        }
    }
}

void printtoplist(){
    cout << "Top List:\n";
    card *ttemp = thead;
    while(ttemp){ //Writing top list
        cout << ttemp->color << "," << ttemp->suit << "," << ttemp->number << "|";
        ttemp = ttemp->next;
    }
    cout << endl;
}

bool readdoc(){
    FILE *oku;
    oku = fopen("solitaire.txt","r");
    if(oku == NULL){
        cout << "Error opening file" << endl;
        getchar();
        return false;
    }
    
    for(int i=0;i<24;i++){ //Reading top list from file
        card* newcard = new card;
        newcard->next = NULL;
        fscanf(oku, "%c %c %s %s \n", &newcard->color, &newcard->suit, newcard->number , newcard->updown);
        if(i==0)
            thead = newcard;
        else
            ttail->next = newcard;
        
        ttail = newcard;
    }

    char a[8];//Passing *******
    fscanf(oku, "%s \n", a);
    //Creating Board List Structure
    //Board list is called list, it
    list = new board;
    tlist = list; //This pointer will help, connecting boards, traverse list
    list->next = NULL;
    list->hcard = NULL;
    list->tcard = NULL;
    //Reading 1.list
    card* newcard = new card;
    newcard->next = NULL;
    fscanf(oku, "%c %c %s %s \n", &newcard->color, &newcard->suit, newcard->number , newcard->updown);
    list->hcard = newcard;
    list->tcard = newcard;
    
    fscanf(oku, "%s \n", a);//passing *******
    //Reading 2.list
    for(int j=2;j<8;j++){
        board *tboard = new board;//Temporary Board Created
        tboard->next = NULL;
        for(int i=0;i<j;i++){ //Reading top list from file
            card *newcard = new card;
            newcard->next = NULL; 
            fscanf(oku, "%c %c %s %s \n", &newcard->color, &newcard->suit, newcard->number , newcard->updown);
            if(i==0)
                tboard->hcard = newcard;
            else
                tboard->tcard->next = newcard;
            
            tboard->tcard = newcard;
        }
        tlist->next = tboard;
        tlist = tboard;
        //Passing *******
        fscanf(oku, "%s \n", a);
    }
    //Creating Foundation List
    createfoundationlist();
    fclose(oku);
    return true;
}

bool movetopfound(card temp){
    ttail = thead;
    card *behind = NULL;
    while(ttail){
        if(temp.color == ttail->color && temp.suit == ttail->suit && temp.number[0] == ttail->number[0]){
            //Fixing the toplist
            if(behind == NULL) //Head
                thead = ttail->next;
            else
                behind->next = ttail->next; 
            
            //Putting into foundation list
            tflist = flist;
            int count = 0; //0 spades, 1 hearts, 2 diamonds,3 clubs 
            char suit = 'S';
            if(ttail->suit == 'H'){
                count = 1;
                suit = 'H';
            }else if(ttail->suit == 'D'){
                count = 2;
                suit = 'D';
            }else if(ttail->suit == 'C'){
                count = 3;
                suit = 'C';
            }
            while(count !=0){
                tflist = tflist->next;
                count--;
            }
            if((tflist->hcard == NULL)){ //No elements in the list
                if((ttail->suit!=suit)||(value(ttail)!=1))
                    return false;
                tflist->hcard = ttail;
                ttail->next = NULL;
                return true;
            }
            tflist->tcard = tflist->hcard;
            while(tflist->tcard->next){
                tflist->tcard = tflist->tcard->next;
            }
            if(value(tflist->tcard)+1!=value(ttail))
                return false;
            tflist->tcard->next = ttail;
            ttail->next = NULL;
            return true;
        }
        behind = ttail;
        ttail = ttail->next;
    }
    return false; //Wrong move or card
    
}

bool topboard(card temp,int dest){ //From top to board
    ttail = thead;
    card *behind = NULL; //Behind of the top list
    while(ttail){
        if(temp.color == ttail->color && temp.suit == ttail->suit && temp.number[0] == ttail->number[0]){
            tlist = list;
            if(dest>7||dest<=0) //If it is valid
                return false;
            for(int i=1;i<dest;i++){
                tlist=tlist->next;
            }
            tlist->tcard = tlist->hcard;
            if(tlist->hcard == NULL){
                if(temp.number[0] == 'K'){ //First member of the board list
                    if(thead->next==NULL){ //First member of the Toplist
                        tlist->hcard = thead;
                        thead = NULL;
                    } else{
                        behind->next = ttail->next;
                        tlist->hcard = ttail;
                        ttail->next = NULL;
                    }
                    return true;   
                }
                return false;
            }else{
                while(tlist->tcard->next){ //Go to the last card
                    tlist->tcard = tlist->tcard->next;
                }
                //tlist->tcard shows the last card
                if((value(tlist->tcard)-1==value(ttail))&&(ttail->suit!=tlist->tcard->suit)){
                    behind->next = ttail->next;
                    tlist->tcard->next = ttail;
                    ttail->next = NULL;
                    return true;
                }
                return false;
            }
            return true;
        }
        
        behind = ttail;
        ttail = ttail->next;
    }
    return false; //Wrong move or card
}

bool moveonboard(int source,int destination,card temp){
    if(source == destination)
        return false;
    board *slist; //Source List
    board *dlist; //Destination List
    //Initializing  Lists to the head board
    tlist = list;
    resetlist(tlist);
    slist = tlist;
    dlist = tlist;
    while(tlist){
        if(source!=1){
            slist = slist->next;
            source--;
        }
        if(destination!=1){
            dlist = dlist->next;
            destination--;
        }
        tlist=tlist->next;
    }
    //Now source and destination shows the right boards
    //Go find the source in the slist but hold the behind card
    card *behinds; //Card behind the required source card
    if(slist->tcard==NULL)
        return false;
    while(!((slist->tcard->color==temp.color)&&(slist->tcard->suit==temp.suit)&&(slist->tcard->number[0]==temp.number[0])&&slist->tcard->updown[0]=='U')){
        behinds = slist->tcard;
        if(slist->tcard->next==NULL)
            return false;
        slist->tcard = slist->tcard->next;
    }
    if(dlist->hcard!=NULL)
        while(dlist->tcard->next)
            dlist->tcard = dlist->tcard->next;
    
    if(dlist->hcard==NULL){
        if(slist->tcard->number[0]=='K'){
            dlist->hcard = slist->tcard;
            slist->hcard = NULL;
        }
        else
            return false;
    }else if((value(dlist->tcard)-1==value(slist->tcard))&&(dlist->tcard->suit!=slist->tcard->suit)){
        dlist->tcard->next = slist->tcard;
        if((slist->hcard->number[0]==slist->tcard->number[0])&&(slist->hcard->color==slist->tcard->color)&&(slist->hcard->suit==slist->tcard->suit))
            slist->hcard=NULL;
    }else
        return false;
    
    //Starting moving operation
    if(behinds->updown[0]=='D')
        strcpy(behinds->updown,"Up");
    behinds->next=NULL;
    return true;
}

bool boardfound(int source){
    tlist = list;
    tflist = flist;
    resetlist(tlist);
    resetlist(tflist);
    while(source!=1){
        source--;
        tlist=tlist->next;
    }
    card *bbehind; //Behind in Board List
    if(tlist->hcard==NULL)
        return false;
    while(tlist->tcard->next){
        bbehind = tlist->tcard;
        tlist->tcard=tlist->tcard->next;
    }
    //Now tcard is the last card in the source list in board
    
    int count = 0; //0 spades, 1 hearts, 2 diamonds,3 clubs 
    char suit = 'S';
    if(tlist->tcard->suit == 'H'){
        count = 1;
        suit = 'H';
    }else if(tlist->tcard->suit == 'D'){
        count = 2;
        suit = 'D';
    }else if(tlist->tcard->suit == 'C'){
        count = 3;
        suit = 'C';
    }
    while(count !=0){
        tflist = tflist->next;
        count--;
    }
    if(tflist->hcard==NULL){
        tflist->hcard=tlist->tcard;
    }else{
        while(tflist->tcard->next)
            tflist->tcard=tflist->tcard->next;
        if(value(tflist->tcard)+1==value(tlist->tcard)){
            tflist->tcard->next=tlist->tcard;
        }else
            return false;
    }
    if(bbehind->updown[0]=='D')
        strcpy(bbehind->updown,"Up");
    bbehind->next = NULL;
    return true;
}

bool print_menu(){
    system("clear");
    //cout << "\n\n\n\n\n";//if system clear doesnt works
    printtoplist();
    printboard();
    printfoundationlist();
    cout << "Choose an operation:\n"
            << "\t1.\tSelect from Top List to Foundation Lists\n"
            << "\t2.\tSelect from Top List to Board Lists\n"
            << "\t3.\tMove on the Board Lists\n"
            << "\t4.\tMove from Board List to Foundation List\n"
            << "\t\tPlease enter your choice (1, 2, 3, or 4):";
    int choice;
    cin >> choice;
    cout << endl;
    card temp; //Temporary card, the one contains users input
    bool success = true;
    int source;
    switch(choice){
        case 1:
            cout << "Select a card from Top List:";
            cin >> temp.color >> temp.suit >> temp.number;
            success = movetopfound(temp);
            if(success)
                cout << "Movement successful\n";
            else
                cout << "Movement is wrong\n";
            break;
        case 2:
            cout << "Select a card from Top List:";
            cin >> temp.color >> temp.suit >> temp.number;
            cout << "Select the number of the destination Board List:";
            int dest_num;
            cin >> dest_num;
            success = topboard(temp,dest_num);
            if(success)
                cout << "Movement successful\n";
            else
                cout << "Movement is wrong\n";
            break;
        case 3:
            int destination;
            cout << "Select the number of the source Board List:";
            cin >> source;
            cout << "Select the number of the destination Board List:";
            cin >> destination;
            cout << "Select a card from the selected source Board List to determine starting point of movement: ";
            cin >> temp.color >> temp.suit >> temp.number;
            success = moveonboard(source,destination,temp);
            if(success)
                cout << "Movement successful\n";
            else
                cout << "Movement is wrong\n";
            break;
        case 4:
            cout << "Select the number of the source Board List:";
            cin >> source;
            success = boardfound(source);
            if(success)
                cout << "Movement successful\n";
            else
                cout << "Movement is wrong\n";
            break;
    }
    
    getchar();
    getchar();
    return true;
}

int main(int argc, char** argv) {
    bool conti = true; //Continue while conti is true
    bool  read = readdoc();
    if(!read) //Cant read close program
        return 0;
    while(conti)
        conti = print_menu();
    return 0;
}



