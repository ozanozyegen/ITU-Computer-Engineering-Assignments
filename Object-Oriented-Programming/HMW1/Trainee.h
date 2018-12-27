/* 
 * File:   Trainee.h
 * Author: flamethrower
 *
 * Created on 23 Mart 2014 Pazar, 02:18
 */

#ifndef TRAINEE_H
#define	TRAINEE_H

class Trainee{
    //Details about the person
    const string name;
    int age;
    const bool gender; //1:female, 0:male
    int height;//In centimetres
    int weight;//In grams
    //Required activity duration per exercise
    int dailyActivityDuration = 30; //In minutes
    Treadmill **treadmillList; //Associates Trainee with the Treadmills
    int numOfTreadmills;
    int totalCaloriesBurned; //In kilocalories
    
public:
    Trainee(string,int,bool,int,int);
    void looseWeight();
    void exercise();
    void addTreadmill(&Treadmill);
    ~Trainee();
};

//Constructor
Trainee::Trainee(const string t_name,int t_age,const bool t_gender,int t_height,int t_weight){
    
    
    // Printing the information about the new object
    if(t_gender == 1)
        cout << "A female trainee is created:\n\n";
    else if(t_gender == 0)
        cout << "A male trainee is created:\n\n";
    else
        cout << "Error: gender variable is not initialized:\n\n";
    
    name = t_name;
    age = t_age;
    gender = t_gender;
    height = t_height;
    weight = t_weight;
    
    cout << "AGE                :" + age;
    cout << "NAME               :" + name;
    cout << "GENDER             :" + gender;
    cout << "HEIGHT             :" + height;
    cout << "WEIGHT             :" + weight;
    cout << "DURATION           :" + dailyActivityDuration;
    
    totalCaloriesBurned = 0;
    numOfTreadmills = 0;
    //Taking memory space for Treadmill
    Treadmill* treadmillList = new Treadmill[3];
    
}
void Trainee::looseWeight(){
    
    if(totalCaloriesBurned == 0 || !totalCaloriesBurned)
        cout << "0 calories were burned.";
    else if(totalCaloriesBurned != 0)
        weight = weight - int(totalCaloriesBurned*(1,5));
    
        totalCaloriesBurned = 0;
  

}
void Trainee::exercise(){
    //Create Later
}
Trainee::~Trainee(){
    delete[] **treadmillList;
}


#endif	/* TRAINEE_H */

