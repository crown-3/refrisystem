#ifndef RECIPERECOMMENDATION_H
#define RECIPERECOMMENDATION_H

#include "Recommendation.h"
#include "Recipe.h"
#include "Interfaces.h"

#include <map>

class Recipe;
class RecipeRecommendation : public Recommendation<RecipeItem> {
    // Current User Mood
    string userMood;

    // RecipeTag Preference Info
    map<string, vector<string>> moodPreference; // Preferred recipe tags for each mood
    map<string, int> userPreference; // Preference Priority of recipe tags for user

    // Reference to Storage instance
    Recipe &recipeRef;

    // recommend helper function
    vector<RecipeItem> moodBasedFilter(vector<RecipeItem> TargetList, string mood);
    void userPreferenceBasedSorter(vector<RecipeItem>& TargetList);
    void makabilityBasedSorter(vector<RecipeItem>& TargetList);
public:
    // Constructor & Destructor
    explicit RecipeRecommendation(Recipe &ref);
    ~RecipeRecommendation();

    // setter & getter
    void setUserMood(string new_userMood);
    void setMoodPreference(map<string, vector<string>> new_moodPreference);
    void setUserPreference(map<string, int> new_userPreference);

    // MAIN recommend function
    vector<RecipeItem> recommend(vector<RecipeItem> TargetList) override;
};

#endif //RECIPERECOMMENDATION_H
