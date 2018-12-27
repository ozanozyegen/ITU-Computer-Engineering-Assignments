/* 
 * File:   liste.h
 * Author: Ozan
 *
 * Created on October 28, 2014, 7:08 PM
 */

#ifndef LISTE_H
#define	LISTE_H

#include <string>
using namespace std;

struct card{
    char color;
    char suit;
    char updown[5];
    char number[3]; //10 is used as 1 in the card node
    card* next;
};


struct board {
    card *hcard; //Holds the head
    card *tcard; //Traverse for travelling in the card list
    board *next;
};



#endif	/* LISTE_H */

