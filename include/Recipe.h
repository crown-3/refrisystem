#ifndef REFRISYSTEM_RECIPE_H
#define REFRISYSTEM_RECIPE_H

#include <vector>
#include "../utils/Table.h"
#include "Storage.h"
#include <nlohmann/json.hpp>
#include "Interfaces.h"
#include "RecipeRecommendation.h"

using namespace std;
using namespace nlohmann;

// 가능한 RecipeTag 종류
extern vector<string> RecipeTagList;
// RecipeTag 중요도 정보
extern map<string, vector<string>> moodPreference; // 기분에 따른 선호 RecipeTag
extern map<int, vector<string>> userPreference;    // 사용자의 RecipeTag 선호순서

// Recipe class
class Recipe {
    string RawJSON_path;
    vector<RecipeItem> recipeData;
    Storage& refrigeratorRef; // Reference to Storage instance

    RecipeRecommendation recommendManager;
public:
    Recipe(string data_path, Storage& ref);
    ~Recipe();

    void loadRecipeData();
    void saveRecipeData(); // save when destructor called

    void showAllRecipe();
    void showPartialRecipe(vector<RecipeItem> targetRecipeData);
    vector<Row> stringifyRecipe(vector<RecipeItem> targetRecipeData); // helper function for showRecipe()

    // --------------------------------------------------------------
    // Add Recipe
    void addRecipe(); // 고쳐야 함
    void showPossibleTags(); // helper function for addRecipe()
    vector<IngredientDetail> inputIngredients(); // helper function for addRecipe()
    vector<string> inputSteps(); // helper function for addRecipe()

    // Remove Recipe
    void removeRecipe(); // 고쳐야 함

    // --------------------------------------------------------------
    // Recommend Recipe
    vector<RecipeItem> recommendRecipe(string mood);
    bool checkMakable(RecipeItem recipe); // helper function

    vector<IngredientDetail> checkLackIngredient(RecipeItem recipe); // helper function
};

#endif //REFRISYSTEM_RECIPE_H