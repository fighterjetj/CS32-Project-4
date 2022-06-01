#include <vector>
#include <set>
#include <string>
#include "MatchMaker.h"
#include "provided.h"
#include "PersonProfile.h"
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include "RadixTree.h"
using namespace std;

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at): database(&mdb), translator(&at){}

MatchMaker::~MatchMaker(){}

vector<EmailCount> MatchMaker::IdentifyRankedMatches(string email, int threshold) const{
    const PersonProfile* thisPerson = database->GetMemberByEmail(email);
    int numAttValPairs = thisPerson->GetNumAttValPairs();
    // RadixTree with keys that are user emails and how many attValPairs they share with the passed user
    RadixTree<int> numMatches;
    set<string> greaterThanEmails;
    // Iterating through every attValPair the passed person has
    for (int i = 0; i < numAttValPairs; i++){
        AttValPair currAV;
        thisPerson->GetAttVal(i, currAV);
        // Iterates through every compatible attValPair
        vector<AttValPair> compatibleAVs = translator->FindCompatibleAttValPairs(currAV);
        for (vector<AttValPair>::iterator compatibleIt = compatibleAVs.begin(); compatibleIt != compatibleAVs.end(); compatibleIt++){
            AttValPair compAV = *compatibleIt;
            // Getting every user that has the compatible AttValPair
            vector<string> compatibleEmails = database->FindMatchingMembers(compAV);
            for (vector<string>::iterator emailIt = compatibleEmails.begin(); emailIt != compatibleEmails.end(); emailIt++){
                string currEmail = *emailIt;
                int* numMatch = numMatches.search(currEmail);
                int newNumMatches;
                // If the returned value is nullptr this email hasn't been encountered yet
                if (numMatch == nullptr){
                    newNumMatches = 1;
                }
                else{
                    newNumMatches = *numMatch + 1;
                }
                numMatches.insert(currEmail, newNumMatches);
                // If the number of matches is equal to our threshold, we add the email to the vector
                if (newNumMatches >= threshold){
                    // Making sure the email isn't the passed email
                    if (email.compare(currEmail)){
                        greaterThanEmails.insert(currEmail);
                    }
                    
                }
            }
        }
    }
    vector<EmailCount> matchesAboveThreshold;
    for (set<string>::iterator greaterIt = greaterThanEmails.begin(); greaterIt != greaterThanEmails.end(); greaterIt++){
        string currEmail = *greaterIt;
        int currCount = *numMatches.search(currEmail);
        EmailCount ec(currEmail, currCount);
        matchesAboveThreshold.push_back(ec);
    }
    return matchesAboveThreshold;
}
