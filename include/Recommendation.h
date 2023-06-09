#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include <vector>
#include <string>

using namespace std;

template <typename Item, typename State, typename Criteria>
class Recommendation {
    // helper function (will be implemented?)
//    virtual vector<Item> stateBasedFilter(vector<Item> TargetList, State UserState, Criteria PriorityCriteria) = 0;
//    virtual vector<Item> priorityBasedSorter(vector<Item> TargetList, State UserState, Criteria PriorityCriteria) = 0;
//    virtual vector<Item> propertyBasedFilter(vector<Item> TargetList, State UserState, Criteria PriorityCriteria) = 0;
protected:
    bool checkSInState
public:
    // Recommendation according to User's state
    // : Returns the TargetList sorted by priorityCriteria for the UserState
    virtual vector<Item> recommend(vector<Item> TargetList) = 0;

};

#endif //RECOMMENDATION_H
