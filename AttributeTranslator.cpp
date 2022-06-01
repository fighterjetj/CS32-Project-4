#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "AttributeTranslator.h"
#include "provided.h"
#include "RadixTree.h"
using namespace std;

AttributeTranslator::AttributeTranslator(){}

AttributeTranslator::~AttributeTranslator(){
}

bool AttributeTranslator::Load(string filename){
    ifstream infile(filename);
    // If it couldn't load the file, returns false; otherwise, will return true
    if (!infile){
        return false;
    }
    string line;
    while (getline(infile, line)){
        // If the length of the line is 0, we do nothing, as that means the line is empty
        if (line.length() > 0){
            // Getting the distance between commas and the attributes and value
            int firstComma = 0;
            while (line[firstComma] != ','){
                firstComma++;
            }
            string firstAtt = line.substr(0,firstComma);
            int secondComma = firstComma + 1;
            while (line[secondComma] != ','){
                secondComma++;
            }
            string firstVal = line.substr(firstComma+1, secondComma-firstComma-1);
            int thirdComma = secondComma + 1;
            while (line[thirdComma] != ','){
                thirdComma++;
            }
            string secondAtt = line.substr(secondComma+1, thirdComma-secondComma-1);
            string secondVal = line.substr(thirdComma+1);
            // Making the AttValPair
            AttValPair av;
            av.attribute = secondAtt;
            av.value = secondVal;
            // Making the key for the radixTree
            ostringstream oss;
            oss << firstAtt << firstVal;
            string key = oss.str();
            // Seeing if a vector already exists for this attribute value pair
            vector<AttValPair>* hobbyVector = compatibleAttVals.search(key);
            //cerr << firstAtt << " " << firstVal << " is compatible with " << secondAtt << " " << secondVal << endl;
            if (hobbyVector == nullptr){
                // If no vector exists, we must make a new one
                //vectorsToDelete.push_back(hobbyVector);
                vector<AttValPair> newVals;
                newVals.push_back(av);
                compatibleAttVals.insert(key, newVals);
            }
            else{
                hobbyVector->push_back(av);
            }
            
        }
    }
    return true;
}

vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const{
    // Making the correct key and then returning the relevant
    ostringstream oss;
    oss << source.attribute << source.value;
    string key = oss.str();
    vector<AttValPair>* toReturn = compatibleAttVals.search(key);
    if (toReturn == nullptr){
        vector<AttValPair> emptyVector;
        return emptyVector;
    }
    else{
        return *toReturn;
    }
}
