#ifndef REFRISYSTEM_CONSTS_H
#define REFRISYSTEM_CONSTS_H

#include <vector>
#include <string>
#include <map>

using namespace std;

// ------------------------------------------------------------------------------------------------------------
// Constants used for Recipe management & recommendation
// ------------------------------------------------------------------------------------------------------------

// Possible Tags for Recipes
vector<string> RecipeTagList = {
        "Vegan", "Vegetarian",  // Preference
        "Cold", "Hot", "Sweet", "Salty", "Spicy", "Sour", "Savory", // Flavor
        "Fat", "Protein", "Carbohydrate", "Vitamin", "Mineral", // Nutrition
        "Healthy", "Premium", // Miscellaneous
};

// Preferred recipe tags for each mood
map<string, vector<string>> moodPreference = {
        {"happy", {"Healthy", "Cold", "Salty", "Sour", "Vitamin", "Mineral"}},
        {"moody", {"Premium", "Sweet", "Savory", "Fat", "Carbohydrate", "Vitamin", "Mineral"}},
        {"exhausted", {"Healthy", "Sweet", "Fat", "Protein", "Carbohydrate"}},
        {"fine", {"Healthy", "Premium", "Cold", "Hot", "Sweet", "Savory", "Protein", "Carbohydrate", "Mineral"}},
        {"mentally_tired", {"Premium", "Hot", "Spicy", "Fat", "Vitamin"}}
};

// Preferred recipe tags for each user
map<int, vector<string>> userPreference = {
        {1, {"Hot", "Sweet", "Salty", "Spicy", "Sour"}},
        {2, {"Premium", "Fat", "Protein", "Carbohydrate"}},
        {3, {"Vitamin", "Mineral", "Savory", "Cold"}},
        {4, {"Healthy", "Vegan", "Vegetarian"}}
};

// ------------------------------------------------------------------------------------------------------------
// Constants used for RefriSystem - User Interaction : Ask List & User Answer & Responses
// ------------------------------------------------------------------------------------------------------------

// [Mood] Question List for User Mood
vector<string> MoodQ = {
        "I'm very happy now.",
        "I feel moody today.",
        "I'm exhausted because of physical movement.",
        "I'm just fine.",
        "I'm mentally tired."
};

// [Mood] Switch number answer to Mood answer
map<int, string> MoodA = {
        {1, "happy"}, {2, "moody"}, {3, "exhausted"}, {4, "fine"}, {5, "mentally_tired"}
};

// [Mood] Responses for each Mood answer
map<string, string> MoodR = {
        {"happy", "Oh, I'm really glad to hear that!"},
        {"moody", "Oh, I understand how you feel."},
        {"exhausted", "Oh, you need to recharge."},
        {"fine", "Oh, that’s nice."},
        {"mentally_tired", "Oh, I'm so sorry to hear that."}
};



#endif //REFRISYSTEM_CONSTS_H
