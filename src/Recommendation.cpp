#include "../include/Recommendation.h"

// ------------------------------------------------------------------------------------------------------------
// **General** helper functions for recommendation (checker-based)
// ------------------------------------------------------------------------------------------------------------

// << booleanCheckerBasedFilter >>
// : Items that checker returns [BooleanCriteria: true/false] are filtered
template<typename Item>
vector<Item> Recommendation<Item>::booleanCheckerBasedFilter(vector<Item> TargetList, function<bool(Item item)> checker,
                                                             BooleanCriteria criteria) {
    vector<Item> filteredList;
    for (const Item &item: TargetList) {
        if ((criteria = TRUE && checker(item)) || (criteria = FALSE && !checker(item))) {
            filteredList.push_back(item);  // If checker returns true, add item to the filtered list
        }
    }
    return filteredList;
}

// << booleanCheckerBasedSorter >>
// : Items that checker returns [BooleanCriteria: true/false] are aligned forward
template<typename Item>
void Recommendation<Item>::booleanCheckerBasedSorter(vector<Item> &TargetList, function<bool(Item item)> checker,
                                                     BooleanCriteria criteria) {
    sort(TargetList.begin(), TargetList.end(), [&](const Item &a, const Item &b) {
        bool a_isValid = checker(a);
        bool b_isValid = checker(b);

        // TRUE/FALSE should come first
        if ((criteria = TRUE && a_isValid && !b_isValid) || (criteria = FALSE && !a_isValid && b_isValid)) {
            return true;
        } else if ((criteria = TRUE && !a_isValid && b_isValid) || (criteria = FALSE && a_isValid && !b_isValid)) {
            return false;
        }
    });
}

// << numericCheckerBasedFilter >>
// : Items that [NumericFilterCriteria: over/under] checking threshold are filtered
template<typename Item>
template<typename NumericCheckingType>
vector<Item> Recommendation<Item>::numericCheckerBasedFilter(vector<Item> TargetList,
                                                             function<NumericCheckingType(Item item)> checker,
                                                             NumericCheckingType threshold,
                                                             NumericFilterCriteria criteria) {
    vector<Item> filteredList;
    for (const Item &item: TargetList) {
        NumericCheckingType value = checker(item);

        if ((criteria = OVER && value > threshold) || (criteria = UNDER && value < threshold)) {
            filteredList.push_back(item);  // If numeric value OVER/UNDER threshold, add item to the filtered list
        }
    }
    return filteredList;
}

// << numericCheckerBasedSorter >>
// : Items are aligned by [NumericSortCriteria: ascending/descending] order with checking count
template<typename Item>
template<typename NumericCheckingType>
void Recommendation<Item>::numericCheckerBasedSorter(vector<Item> &TargetList,
                                                     function<NumericCheckingType(Item item)> checker,
                                                     NumericSortCriteria criteria) {
    sort(TargetList.begin(), TargetList.end(), [&](const Item &a, const Item &b) {
        NumericCheckingType a_value = checker(a);
        NumericCheckingType b_value = checker(b);

        if (criteria == ASCENDING) return a_value < b_value; // Sort in ascending order based on the numeric value
        else if (criteria == DESCENDING)
            return a_value > b_value;  // Sort in descending order based on the numeric value
    });
}

