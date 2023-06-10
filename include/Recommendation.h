#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include <vector>
#include <string>
#include <functional>

using namespace std;

template <typename Item>
class Recommendation {
protected:
    enum BooleanCriteria {TRUE, FALSE};
    enum NumericFilterCriteria {OVER, UNDER};
    enum NumericSortCriteria {ASCENDING, DESCENDING};

    // recommend **General** helper function
    vector<Item> booleanCheckerBasedFilter(vector<Item> TargetList, function<bool(Item item)> checker, BooleanCriteria criteria);
    void booleanCheckerBasedSorter(vector<Item>& TargetList, function<bool(Item item)> checker, BooleanCriteria criteria);
    template <typename NumericCheckingType>
    vector<Item> numericCheckerBasedFilter(vector<Item> TargetList, function<NumericCheckingType(Item item)> checker, NumericCheckingType threshold, NumericFilterCriteria criteria);
    template <typename NumericCheckingType>
    void numericCheckerBasedSorter(vector<Item>& TargetList, function<NumericCheckingType(Item item)> checker, NumericSortCriteria criteria);
public:
    // Main(Total) Recommendation function
    // : Returns the TargetList sorted by distinct priorityCriteria
    virtual vector<Item> recommend(vector<Item> TargetList) = 0;
};

#endif //RECOMMENDATION_H
