#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "MemberDatabase.h"
#include "provided.h"
#include "PersonProfile.h"
#include "RadixTree.h"
using namespace std;

MemberDatabase::MemberDatabase(){}

MemberDatabase::~MemberDatabase(){
    vector<PersonProfile*>::iterator it = profilesToDelete.begin();
    while(it != profilesToDelete.end()){
        delete *it;
        profilesToDelete.erase(it);
        it = profilesToDelete.begin();
    }
}

bool MemberDatabase::LoadDatabase(std::string filename){
    ifstream infile(filename);
    // If we couldn't load the file, returns false
    if (!infile){
        return false;
    }
    string line;
    while (getline(infile, line)){
        // We know the order of attributes
        string name = line;
        getline(infile, line);
        string email = line;
        // If there already exists a member with the email given, returns false
        if (allMembers.search(email) != nullptr){
            return false;
        }
        PersonProfile* newMember = new PersonProfile(name, email);
        profilesToDelete.push_back(newMember);
        allMembers.insert(email, newMember);
        getline(infile, line);
        int numAttVal = stoi(line);
        for (int i = 0; i < numAttVal; i++){
            getline(infile, line);
            // Gets the location of the comma to divide up the attribute and the value
            int commaIndex = 0;
            while (line[commaIndex] != ','){
                commaIndex++;
            }
            string attribute = line.substr(0, commaIndex);
            string value = line.substr(commaIndex+1);
            AttValPair av(attribute, value);
            newMember->AddAttValPair(av);
            // Adds the newMember to the radixTree of members with this attribute
            ostringstream oss;
            oss << attribute << value;
            string key = oss.str();
            // Gets the vector corresponding to this attValPair
            vector<PersonProfile*>* attVector = attributeMembers.search(key);
            // Adds the pointer to the PersonProfile to the vector
            if (attVector == nullptr){
                // If the vector doesn't exist, then we need to make it
                vector<PersonProfile*> newVector;
                newVector.push_back(newMember);
                attributeMembers.insert(key, newVector);
            }
            else{
                attVector->push_back(newMember);
            }
        }
        // Skipping the empty line
        getline(infile, line);
    }
    return true;
}


vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const{
    ostringstream oss;
    oss << input.attribute << input.value;
    string key = oss.str();
    vector<PersonProfile*>* memberVector = attributeMembers.search(key);
    vector<string> toReturn;
    // If no vector exists, then we must return an empty vector
    if (memberVector == nullptr){
        return toReturn;
    }
    // Otherwise we must make the return vector
    for (vector<PersonProfile*>::iterator it = memberVector->begin(); it != memberVector->end(); it++){
        toReturn.push_back((*it)->GetEmail());
    }
    return toReturn;
}


const PersonProfile* MemberDatabase::GetMemberByEmail(string email) const{
    PersonProfile** profile = allMembers.search(email);
    if (profile != nullptr){
        return *profile;
    }
    else{
        return nullptr;
    }
}

