#ifndef REFRISYSTEM_RECIPE_H
#define REFRISYSTEM_RECIPE_H

#include <vector>
#include "../utils/Table.h"
#include "Storage.h"
#include <nlohmann/json.hpp>
#include "Interfaces.h"
#include "RecipeRecommendation.h"
#include "RecipeDataManagement.h"

using namespace std;
using namespace nlohmann;

// 가능한 RecipeTag 종류
extern vector<string> RecipeTagList;
// RecipeTag 중요도 정보
//extern map<string, vector<string>> moodPreference; // 기분에 따른 선호 RecipeTag
//extern map<int, vector<string>> userPreference;    // 사용자의 RecipeTag 선호순서

// Recipe class
class RecipeRecommendation;
class Recipe {
private:
    string RawJSON_path;
    RecipeDataManagement* dataManager;
    RecipeRecommendation* recommendManager;
    Storage &storageRef; // Reference to Storage instance
public:
    Recipe(string data_path, Storage &ref);
    ~Recipe();

    void showAllRecipe();
    void showPartialRecipe(vector<RecipeItem> targetRecipeData);

    bool checkRecipeExist(string recipeName);
    vector<Row> stringifyRecipeForTable(vector<RecipeItem> targetRecipeData); // helper function for showRecipe()

    // ------------------------------------------------------------------------------------------------------------
    // Add Recipe
    // ------------------------------------------------------------------------------------------------------------
    void addRecipeSequence();

    void printPossibleTags(); // helper function for addRecipe()
    vector<IngredientDetail> inputIngredients(); // helper function for addRecipe()
    vector<string> inputSteps(); // helper function for addRecipe()

    // ------------------------------------------------------------------------------------------------------------
    // Remove Recipe
    // ------------------------------------------------------------------------------------------------------------
    void removeRecipeSequence();

    // ------------------------------------------------------------------------------------------------------------
    // Recommend Recipe
    // ------------------------------------------------------------------------------------------------------------
    vector<RecipeItem> recommendRecipe(string mood);

    // ------------------------------------------------------------------------------------------------------------
    // Inspect Recipe
    // ------------------------------------------------------------------------------------------------------------
    void inspectRecipeSequence();


    // helper functions for RecipeRecommendation
    bool checkMakable(RecipeItem recipe);
    int countLackIngredients(RecipeItem recipe);
    vector<IngredientDetail> checkLackIngredients(RecipeItem recipe);
};

#endif //REFRISYSTEM_RECIPE_H