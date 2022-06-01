#ifndef Matchmaker_h
#define Matchmaker_h
#include <vector>
#include <string>
#include "provided.h"
#include "PersonProfile.h"
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include "RadixTree.h"

class MatchMaker{
public:
    MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
    ~MatchMaker();
    std::vector<EmailCount> IdentifyRankedMatches(std::string email,int threshold) const;
private:
    const MemberDatabase* database;
    const AttributeTranslator* translator;
};

#endif
