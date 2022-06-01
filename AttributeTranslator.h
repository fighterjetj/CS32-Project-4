#ifndef ATTRIBUTETRANSLATOR_H
#define ATTRIBUTETRANSLATOR_H
#include <string>
#include <vector>
#include "provided.h"
#include "RadixTree.h"

class AttributeTranslator{
public:
    AttributeTranslator();
    ~AttributeTranslator();
    bool Load(std::string filename);
    std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
private:
    RadixTree<std::vector<AttValPair>> compatibleAttVals;
//    std::vector<std::vector<AttValPair>*> vectorsToDelete;
};

#endif
