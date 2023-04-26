//
//  utility.cpp
//  Project 4
//
//  Created by Theodore Patsis on 15/3/23.
//

#include "utility.h"
#include <vector>
#include <string>
using namespace std;

string convert_to_lower(string s) { 
    for (int i=0; i<s.size(); i++){
        s.at(i)=tolower(s.at(i));
    }
    return s;
}

vector<string> convert_to_lower (vector<string> v) {
    int j=v.size();
    vector<string> v_lower;
    for (int i=0; i<j; i++){
        string temp=v[i];
        temp=convert_to_lower(temp);
        v_lower.push_back(temp);
    }
    return v_lower;
}
