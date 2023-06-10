#include "../include/RecipeRecommendation.h"

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <functional>

using namespace std;

// ------------------------------------------------------------------------------------------------------------
// Constructor & Destructor & setter & getter
// ------------------------------------------------------------------------------------------------------------

RecipeRecommendation::RecipeRecommendation(Recipe &ref)
: userMood("none"), recipeRef(ref)
{
    setMoodPreference ({
            {"happy", {"Healthy", "Cold", "Salty", "Sour", "Vitamin", "Mineral"}},  // preferred recipeTag when happy
            {"moody", {"Premium", "Sweet", "Savory", "Fat", "Carbohydrate", "Vitamin", "Mineral"}},  // preferred recipeTag when moody
            {"exhausted", {"Healthy", "Sweet", "Fat", "Protein", "Carbohydrate"}},  // preferred recipeTag when exhausted
            {"fine", {"Healthy", "Premium", "Cold", "Hot", "Sweet", "Savory", "Protein", "Carbohydrate", "Mineral"}},  // preferred recipeTag when fine
            {"mentally_tired", {"Premium", "Hot", "Spicy", "Fat", "Vitamin"}}  // preferred recipeTag when mentally_tired
    });
    setUserPreference ({
        // {"TagName", preferenceScore} : The higher the preferenceScore, the more the user prefers the tag
        {"Vegan", 1}, {"Vegetarian", 2},  // tags related to Dietary pattern
        {"Cold", 3}, {"Hot", 4}, {"Sweet", 4}, {"Salty", 3}, {"Spicy", 5}, {"Sour", 1}, {"Savory", 4}, // tags related to Flavor
        {"Fat", 4}, {"Protein", 2}, {"Carbohydrate", 3}, {"Vitamin", 1}, {"Mineral", 1}, // tags related to Nutrition
        {"Healthy", 4}, {"Premium", 5} // tags related to Miscellaneous
    });
}

RecipeRecommendation::~RecipeRecommendation() {

}

void RecipeRecommendation::setUserMood(string new_userMood) {
    // validation check for userMood
    bool isValid = true;

    if (isValid) {
        userMood = new_userMood;
    } else {
        throw invalid_argument("userMood setter validation check Error");
    }
}

void RecipeRecommendation::setMoodPreference(map<string, vector<string>> new_moodPreference) {
    // validation check for moodPreference
    bool isValid = true;

    if (isValid) {
        moodPreference = new_moodPreference;
    } else {
        throw invalid_argument("moodPreference setter validation check Error");
    }
}

void RecipeRecommendation::setUserPreference(map<string, int> new_userPreference) {
    // validation check for userPreference
    bool isValid = true;

    for (const auto& map : new_userPreference) {
        // 1. Check if the key is in RecipeTagList
        if(find(RecipeTagList.begin(), RecipeTagList.end(), map.first) == RecipeTagList.end()) {
            isValid = false;
            break;
        }
        // 2. Check if the value is between 1 and 5
        if(map.second < 1 || map.second > 5) {
            isValid = false;
            break;
        }
    }
    // 3. Check if all tags from RecipeTagList are present in the user preferences
    for(const auto& tag : RecipeTagList){
        if(new_userPreference.find(tag) == new_userPreference.end()){
            isValid = false;
            break;
        }
    }

    if (isValid) {
        userPreference = new_userPreference;
    } else {
        throw invalid_argument("userPreference setter validation check Error");
    }
}

// ------------------------------------------------------------------------------------------------------------
// Functions for Recipe recommendation
// ------------------------------------------------------------------------------------------------------------

// MAIN FUNCTION for RecipeRecommendation
// << recommend >>
// : return recipeRecommendationList that is filtered & sorted by some criteria
vector<RecipeItem> RecipeRecommendation::recommend(vector<RecipeItem> TargetList) {
    vector<RecipeItem> recipeRecommendationList;

    // step 0: userMood setted check
    if (userMood == "none") {
        throw runtime_error("UserMood for recomendation is not set upped. ");
    }

    // Step 1: Select recipe items with tags matching with moodPreference
    recipeRecommendationList = moodBasedFilter(TargetList, userMood);

    // Step 2: Sort recipe items based on userPreference order
    userPreferenceBasedSorter(recipeRecommendationList);

    // Step 3: Sort makable(lower count of lackIngridient) recipes first
    makabilityBasedSorter(recipeRecommendationList);

    return recipeRecommendationList;
}

// << moodBasedFilter >>
// : Filter only the recipe with the preferred recipeTags according to the user's current mood
vector<RecipeItem> RecipeRecommendation::moodBasedFilter(vector<RecipeItem> TargetList, string mood) {
    return Recommendation::booleanCheckerBasedFilter(
            TargetList,
            [=](const RecipeItem& recipe) {
                bool hasMatchingTag = false;

                const auto& preference = moodPreference.find(mood);
                if (preference == moodPreference.end()) {
                    throw runtime_error("There is no mood corresponding to the key of map 'moodPreference'");
                }
                const auto& preferenceTags = preference->second;

                for (const auto& tag : recipe.tags) {
                    if (find(preferenceTags.begin(), preferenceTags.end(), tag) != preferenceTags.end()) {
                        hasMatchingTag = true;
                        break;
                    }
                }

                return hasMatchingTag;
            },
            TRUE
    );
}

// << userPreferenceBasedSorter >>
// : Sort the recipe in priority of favorite tags, based on user preference score
void RecipeRecommendation::userPreferenceBasedSorter(vector<RecipeItem>& TargetList) {
    Recommendation::numericCheckerBasedSorter<double>(
            TargetList,
            [=](const RecipeItem& recipe) {
                int totalPreferenceScore = 0;
                int tagCount = 0;

                for (const auto& tag : recipe.tags) {
                    tagCount += 1;
                    totalPreferenceScore += userPreference[tag];
                }

                if (tagCount <= 0) {
                    return static_cast<double>(0);
                } else {
                    return static_cast<double>(totalPreferenceScore) / tagCount;
                }
            },
            DESCENDING
    );
}

// << makabilityBasedSorter >>
// : Sort the recipe checking makability (higher makablility : lower count of lackIngridient)
void RecipeRecommendation::makabilityBasedSorter(vector<RecipeItem>& TargetList) {
    Recommendation::numericCheckerBasedSorter<double>(
            TargetList,
            [=](const RecipeItem& recipe) {
                return recipeRef.countLackIngredients(recipe);
            },
            ASCENDING
    );
}

