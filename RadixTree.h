#ifndef RADIXTREE_H
#define RADIXTREE_H

#include <iostream>
#include <string>
#include <vector>

template <typename ValueType>

class RadixTree {
public:
    RadixTree();
    ~RadixTree();
    void insert(std::string key, const ValueType& value);
    ValueType* search(std::string key) const;
    void printTree();
private:
    void resetSubtries();
    std::string stringSlice;
    std::vector<RadixTree*> subTries;
    bool hasValue;
    ValueType correspondingVal;
};

template<typename ValueType>
inline
RadixTree<ValueType>::RadixTree(): hasValue(false){
    resetSubtries();
}

template<typename ValueType>
inline
RadixTree<ValueType>::~RadixTree(){
    for (int i = subTries.size()-1; i >= 0; i--){
        delete subTries[i];
    }
}

template<typename ValueType>
inline
void RadixTree<ValueType>::insert(std::string key, const ValueType &value){
    // Check if there is a radixtree for the first letter in the string
    char startChar = key[0];
    RadixTree* subTree = subTries[startChar];
    if (subTree == nullptr){
        RadixTree* newTrie = new RadixTree();
        newTrie->stringSlice = key;
        newTrie->hasValue = true;
        newTrie->correspondingVal = value;
        subTries[startChar] = newTrie;
    }
    // Otherwise there is an existing radixtree at the corresponding spot
    else{
        std::string radixString = subTree->stringSlice;
        // If the stringslice has a length of 1, we know we do not need to divide the slice up, because it is just one letter
        if (radixString.length() == 1){
            // If the key also has a length of 1, then the key and the substring match so we can just set this value
            if (key.length() == 1){
                subTree->hasValue = true;
                subTree->correspondingVal = value;
            }
            // If the key does not have a length of 1, we simply try and insert it into the subTree
            else{
                subTree->insert(key.substr(1), value);
            }
        }
        // There is a chance we will need to divide the corresponding substring because it is longer than one character
        else{
            // Iterates through the substring until one of the strings diverges from the other or they reach the end of the string
            int lastSame = 1;
            while (key[lastSame] == radixString[lastSame] && (key[lastSame] != '\0') && (radixString[lastSame] != '\0')) {
                lastSame++;
            }
            std::string sharedPart = key.substr(0, lastSame);
            // Getting the two substrings of the non-shared part of the string
            std::string keyRemains = key.substr(lastSame);
            std::string radRemains = radixString.substr(lastSame);
            // If we iterated through and both strings are the same, we simply reassign the value
            if ((key[lastSame] == '\0') && (radixString[lastSame] == '\0')){
                subTree->hasValue = true;
                subTree->correspondingVal = value;
            }
            // If the subtree's string was iterated all the way through, then we know that the rest of the key is simply appended to the end
            else if (radixString[lastSame] == '\0'){
                subTree->insert(keyRemains, value);
            }
            /* We need to 'break' this node into two parts, the shared part and the non-shared part
             so we will make a new RadixTree for just the shared part, make the old tree have just the non-shared part,
             add the old tree to the new one, and then insert the non-shared part of the key */
            else {
                RadixTree* sharedString = new RadixTree();
                sharedString->stringSlice = sharedPart;
                subTree->stringSlice = radRemains;
                // Adding the old tree to the new one
                sharedString->subTries[radRemains[0]] = subTree;
                // If the key has ended, we just add its value to the shared string, as that encompasses the entire key
                if (key[lastSame] == '\0'){
                    sharedString->hasValue = true;
                    sharedString->correspondingVal = value;
                }
                else{
                    sharedString->insert(keyRemains, value);
                }
                subTries[startChar] = sharedString;
            }
        }
    }
}

template<typename ValueType>
inline
ValueType* RadixTree<ValueType>::search(std::string key) const{
    RadixTree* subTree = subTries[key[0]];
    // If there is a nullptr then there are no values down this path
    if (subTree == nullptr){
        return nullptr;
    }
    else{
        // We compare the stringSlice to the key's corresponding starting values, and if they are not equal we know no corresponding value exists
        std::string subSlice = subTree->stringSlice;
        // If the key is shorter than the corresponding string, then no value exists down this path
        if (subSlice.length() > key.length()){
            return nullptr;
        }
        if (!subSlice.compare(key.substr(0, subSlice.length()))){
            // If there is more of the string left, we must continue to search it
            if (subSlice.length() < key.length()){
                return subTree->search(key.substr(subSlice.length()));
            }
            // Otherwise the two strings must have the same length
            else{
                if (subTree->hasValue){
                    return &(subTree->correspondingVal);
                }
                else{
                    return nullptr;
                }
            }
        }
        else{
            return nullptr;
        }
    }
}

template<typename ValueType>
inline
void RadixTree<ValueType>::resetSubtries(){
    subTries.clear();
    // Filling the tree with nullptrs because there is no data in it currently, and there are 128 possible ascii characters
    for (int i = 0; i < 128; i++){
        subTries.push_back(nullptr);
    }
}

template<typename ValueType>
inline
void RadixTree<ValueType>::printTree(){
    std::cerr << "We have the string |" << stringSlice << "| and its kids are: ";
    for (int i = 0; i < 128; i++){
        if (subTries[i] != nullptr){
            std::cerr << "'" << subTries[i]->stringSlice << "' ";
        }
    }
    std::cerr << std::endl;
    for (int i = 0; i < 128; i++){
        if (subTries[i] != nullptr){
            char correspChar = i;
            std::cerr << "The child of |" << stringSlice << "| '" << subTries[i]->stringSlice << "' Which should start with " << correspChar << std::endl;
            subTries[i]->printTree();
        }
    }
}

#endif
