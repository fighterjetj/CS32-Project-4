#ifndef MemberDatabase_h
#define MemberDatabase_h
#include <string>
#include <vector>
#include "provided.h"
#include "PersonProfile.h"
#include "RadixTree.h"

class MemberDatabase{
public:
    MemberDatabase();
    ~MemberDatabase();
    bool LoadDatabase(std::string filename);
    std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
    const PersonProfile* GetMemberByEmail(std::string email) const;
private:
    RadixTree<PersonProfile*> allMembers;
    RadixTree<std::vector<PersonProfile*>> attributeMembers;
    std::vector<PersonProfile*> profilesToDelete;
};

#endif
