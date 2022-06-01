#include <iostream>
#include <sstream>
#include <vector>
#include "provided.h"
#include "PersonProfile.h"
using namespace std;

PersonProfile::PersonProfile(string name, string email):name(name), email(email){}

string PersonProfile::GetName() const{
    return name;
}

string PersonProfile::GetEmail() const{
    return email;
}

int PersonProfile::GetNumAttValPairs() const{
    return radixPaths.size();
}

void PersonProfile::AddAttValPair(const AttValPair &attval){
    // Increases the number of pairs
    ostringstream oss;
    oss << attval.attribute<< attval.value;
    string pathName = oss.str();
    // If the current attribute doesn't already exist, we must add it
    if (attVals.search(pathName) == nullptr){
        // Adds the path name to the vector of path names
        radixPaths.push_back(attval);
        // Adds the attval object to the radixTree of attVals
        attVals.insert(pathName, true);
    }
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const{
    // If the number is greater than or equal to the number of attval pairs, we cannot get any value so we immediately return false
    if ((attribute_num >= radixPaths.size()) || (attribute_num < 0)){
        return false;
    }
    else{
        // Copies the value with the path at the radixPath index of attribute_num
        attval = radixPaths[attribute_num];
        
        return true;
    }
}
