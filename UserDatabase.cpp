#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

UserDatabase::UserDatabase()
{
    loaded=false;
}

UserDatabase::~UserDatabase(){
    for (int i=0; i<to_delete.size(); i++){
        delete to_delete[i];
    }
}

bool UserDatabase::load(const string& filename)
{
    if(loaded){
        return false;
    }
    ifstream file(filename);
    for(;;) {
        string name="";
        getline(file, name);
        if (name.empty()) { // check if the name string is empty
            break; // exit loop if the end of the file has been reached
        }
        string email="";
        getline(file, email);
        string num="";
        getline(file, num);
        int n=convert(num);
        vector<string> watch_history;
        for (int i=0; i<n; i++){ //reads in each one of the watch history movie IDs
            string temp;
            getline(file,temp);
            watch_history.push_back(temp);
        }
        string line;
        getline(file,line); //advances the empty line
        //create a user pointer with the data from the text file
        User* ptr=new User(name, email, watch_history);
        //add to multimap
        user_map.insert(email, ptr);
        //add in the user pointer in a container of objects to be destructed when they go out of scope
        to_delete.push_back(ptr);
    }
    loaded=true;
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string, User*>::Iterator it=user_map.find(email);
    return it.get_value();
}

int UserDatabase::convert(const string s) const{ //converts a string to an int
    int total=0;
    for (int i=s.size()-1; i>=0; i--){
        char c=s.at(i);
        int temp=c-'0';
        total+=temp*pow(10, s.size()-1-i);
    }
    return total;
}
