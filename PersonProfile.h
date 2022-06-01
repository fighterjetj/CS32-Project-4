#ifndef PERSONPROFILE_H
#define PERSONPROFILE_H

#include <iostream>
#include <vector>
#include "RadixTree.h"
#include "provided.h"

class PersonProfile{
public:
    PersonProfile(std::string name, std::string email);
    std::string GetName() const;
    std::string GetEmail() const;
    int GetNumAttValPairs() const;
    void AddAttValPair(const AttValPair& attval);
    bool GetAttVal(int attribute_num, AttValPair& attval) const;
private:
    std::string name;
    std::string email;
    RadixTree<bool> attVals;
    std::vector<AttValPair> radixPaths;
};

#endif
