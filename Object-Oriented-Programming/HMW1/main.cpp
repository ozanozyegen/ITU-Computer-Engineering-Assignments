/* 
 * File:   main.cpp
 * Author: Ozan Ozyegen
 *
 * Created on 12 Mart 2014 Çarşamba, 20:48
 */

#include<iostream>
#include<string>
// Other includes

using namespace std;

/*
 * 
 */
class Treadmill{
    static int numOfTreadmills;
    int id;
    bool availability;
public:
    Treadmill();
    bool isAvailable();
    int getId();
    int run(bool, int, int, int);//Input parameters in order are: gender, age, weight of caller trainee and duration of exercise.
    //Run function sets availability to true before return.
    void unsetAvailability();
    ~Treadmill();
    static void resetNumOfTreadmills(){numOfTreadmills = 0;};// Resetting number of treadmills to 0. 
    
};
Treadmill::Treadmill(){
    availability = true;
    numOfTreadmills++;
    id = numOfTreadmills;
    cout << "Treadmill with id: " << id << " is created\n";
}
bool Treadmill::isAvailable(){
    return availability;
}
int Treadmill::getId(){
    return id;
}
int Treadmill::run(bool gender, int age, int weight, int dailyActivityDuration){
    availability = false;
    if(gender == 1){
        availability = true;
        return ((-20.4022 + 0.4472*100 - 0.1263*(weight/1000) + 0.074*age)/4.184)*dailyActivityDuration;}
    else if(gender == 0){
        return ((-55.0969+0.6309*100+0.1988*weight+0.2017*age)/4.184)*dailyActivityDuration;
    }
        
}
void Treadmill::unsetAvailability(){
    availability = false;
}
Treadmill::~Treadmill(){
    cout << "Treadmill with id: " << id << " is deleted\n";
    numOfTreadmills = numOfTreadmills - 1;
}

class Trainee{
    //Details about the person
    const string name;
    int age;
    const bool gender; //1:female, 0:male
    int height;//In centimetres
    int weight;//In grams
    //Required activity duration per exercise
    int dailyActivityDuration; //In minutes
    Treadmill **treadmillList; //Associates Trainee with the Treadmills
    int numOfTreadmills;
    int totalCaloriesBurned; //In kilocalories
    
public:
    Trainee(const string,int,const bool,int,int,int);
    void looseWeight();
    void exercise();
    bool addTreadmill(Treadmill*);
    ~Trainee();
};

//Constructor
Trainee::Trainee(const string t_name,int t_age,const bool t_gender,int t_height,int t_weight,int t_daily_act = 30): name(t_name),gender(t_gender) { //Constant members initialized here
    
    
    // Printing the information about the new object
    if(t_gender == 1)
        cout << "A female trainee is created:\n\n";
    else if(t_gender == 0)
        cout << "A male trainee is created:\n\n";
    else
        cout << "Error: gender variable is not initialized:\n\n";
    
    
    age = t_age;
    height = t_height;
    weight = t_weight;
    this->dailyActivityDuration = t_daily_act;
    cout << "AGE           :" << age <<endl;
    cout << "NAME          :" << name <<endl;
    cout << "GENDER        :" << gender <<endl;
    cout << "HEIGHT        :" << height <<endl;
    cout << "WEIGHT        :" << weight <<endl;
    cout << "DURATION      :" << dailyActivityDuration << endl;
    
    
    numOfTreadmills = 0;
    totalCaloriesBurned = 0;
    //Taking memory space for Treadmill
    treadmillList = new Treadmill*[3];
    
}
void Trainee::looseWeight(){
    
    if(totalCaloriesBurned == 0 || !totalCaloriesBurned)
        cout << "0 calories were burned. \n";
    else if(totalCaloriesBurned != 0){
        cout << "Your weight was :" << weight/1000.0<<endl;
        weight = weight - int(totalCaloriesBurned*(1.5));
        cout << "Now: " << weight/1000.0 << "kg...\n";
    }
        
    
        totalCaloriesBurned = 0;
  

}
void Trainee::exercise(){
    for(int i=0;i<numOfTreadmills;i++){
        if(treadmillList[i]->isAvailable()){
            cout << "An available treadmill found having id: " << treadmillList[i]->getId()<<endl;
            cout << "Exercising for " << dailyActivityDuration << "minutes\n";
            totalCaloriesBurned = treadmillList[i]->run(gender,age,weight,dailyActivityDuration);
            cout << "Total Kilocalories burned: " << totalCaloriesBurned <<endl;
            break;
        }
        else if(i == numOfTreadmills-1){
            cout << "None of the Treadmills are available" <<endl;
        }
        else if(numOfTreadmills == 0){
            cout << "Sorry, no treadmills are related to trainee" << name << ", can NOT exercise";
        }
    }
}
bool Trainee::addTreadmill(Treadmill* tM){
    if(numOfTreadmills < 3)
    {
    treadmillList[numOfTreadmills] = tM;
    numOfTreadmills++;
    return true;
    }
    return false;
}
Trainee::~Trainee(){
    delete[] treadmillList;
    
}






int Treadmill::numOfTreadmills;

int main(int argv, char * argc[]) {
    Treadmill::resetNumOfTreadmills(); // Resetting number of treadmills to 0. 
    
    //Creating Treadmills for the gym
    Treadmill tM1;
    Treadmill tM2;
    Treadmill tM3;
    Treadmill tM4;
    {
        Treadmill tM5;
    }
    Treadmill tM6;
    
    // Creating a Trainee: (dailyActivityDuration is 30 minutes as default)
    // Check Constructor messages
    Trainee trainee1 ("Sally Brown", 18, 1, 170, 55000);
    // Trainee trainee2 = trainee1; 
    // When uncommented, should work properly
    // Try a daily exercise before treadmill/s are related with this trainee:
    if (!trainee1.addTreadmill(&tM1)) cout << "Add Operation 1 is not successfull" << endl;
    if (!trainee1.addTreadmill(&tM2)) cout << "Add Operation 2 is not successfull" << endl;
    if (!trainee1.addTreadmill(&tM3)) cout << "Add Operation 3 is not successfull" << endl;
    
    // Should print out error message since max list size is 3.
    if (!trainee1.addTreadmill(&tM4)) cout << "Add Operation 4 is not successfull" << endl << endl;
    
    // Try to loose weight before exercise :
    trainee1.looseWeight();
    
    // SAMPLE Workout:
    trainee1.exercise();
    
    // Update Weight:
    trainee1.looseWeight();
    
    // What if all treadmills are unavailable?
    tM1.unsetAvailability();
    tM2.unsetAvailability();
    tM3.unsetAvailability();
    
    trainee1.exercise();
    return 0;
}