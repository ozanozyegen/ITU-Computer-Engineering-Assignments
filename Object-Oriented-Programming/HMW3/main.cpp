/* 
 * File:   main.cpp
 * Author: Ozan Özyeğen
 * Istanbul Technical University - BLG 252E Homework 3
 * Created on 11 Mayıs 2014 Pazar, 12:08
 */

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

/* 
 *  There are two classes Quote and Movie.
 *  Each quote is a line from a movie.
 *  It has an owner who recites the line, and there’s
 *  one and only one movie which it belongs to.
 *  Movie has a single title and a list of quotes as 
 *  attributes.
 */
class Quote{
    string line;
    string owner;
    string movie;
public:
    Quote(){}
    Quote(const string q):line(q){}
    Quote(const string q,const string o, const string m):line(q),owner(o),movie(m){}//Quote,Owner,Movie
    void print() const;
    void set_quote(const string q){line=q;}//Quote
    void set_owner(const string o){owner=o;}//Owner
    void set_movie(const string m){movie=m;}//Movie
    string get_movie()const{return movie;}
};
void Quote::print() const{
    cout << line << endl;
    cout << "    (" <<owner<<" - "<<movie<<")\n";
}

class Movie{
    string title;
    vector<Quote> quotes;
public:
    Movie(){}
    Movie(const string t):title(t){}
    void set_title(const string t){title=t;}
    void operator+(const Quote&);
    void print() const;
};
void Movie::operator +(const Quote& q){
    if(title!=q.get_movie()){throw "insertion error: titles mismatch";}
    quotes.push_back(q);
}
void Movie::print() const{
    for(int i=0;i<quotes.size();i++){
        quotes[i].print();
    }
}

int main(int argc, char** argv) {
    Quote q1;
    q1.set_quote("it's a trap");
    q1.set_owner("admiral ackbar");
    q1.set_movie("return of the jedi");
    Quote q2("eight year old dude...","walter sobchak","the big lebowski");
    Quote q3("if you want to survive out here, you've got to know where your towel is.");
    q3.set_owner("ford prefect");
    q3.set_movie("the hitchhiker's guide to the galaxy");
    Quote q4("this will all end in tears.","marvin","the hitchhiker's guide to the galaxy");
    Quote q5("what does marcellus wallace look like?","jules winfield","pulp fiction");
    Movie m1;
    m1.set_title("reservoir dogs");
    Movie m2("the hitchhiker's guide to the galaxy");
    Movie m3("the big lebowski");
    try{m2 + q3;}catch(const char *m){cout << m << endl;}
    try{m2 + q4;}catch(const char *m){cout << m << endl;}
    try{m3 + q2;}catch(const char *m){cout << m << endl;}
    try{m1 + q5;}catch(const char *m){cout << m << endl;}
    m2.print();
    return 0;
}

