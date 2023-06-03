#ifndef REFRISYSTEM_REFRIRECIPE_H
#define REFRISYSTEM_REFRIRECIPE_H

#include <vector>
#include "../hooks/Table.h"
#include "Refrigerator.h"
#include <nlohmann/json.hpp>
using namespace std;
using namespace nlohmann;

// 가능한 RecipeTag 종류
extern vector<string> RecipeTagList;
// RecipeTag 중요도 정보
extern map<string, vector<string>> moodPreference; // 기분에 따른 선호 RecipeTag
extern map<int, vector<string>> userPreference;    // 사용자의 RecipeTag 선호순서

// RecipeRow 틀
struct IngredientDetail;
struct RecipeRow {
    int id;
    string name;
    vector<string> tags;
    vector<string> steps;
    vector<IngredientDetail> ingredients;
};
struct IngredientDetail {
    string name;
    double amount;
};

// RefriRecipe 클래스
class RefriRecipe {
    string RawJSON_path;
    vector<RecipeRow> recipeData;
    Refrigerator& refrigeratorRef; // Reference to Refrigerator instance
public:
    // Load from JSON file
    RefriRecipe(string data_path, Refrigerator& ref);  // json에서 벡터변수로 parsing해서 load
    void loadRecipeData(); // helper funtion for constructor

    // Save to JSON file
    ~RefriRecipe(); // json에 변경사항 save
    void saveRecipeData(); // helper function for destructor

    // Show Recipes
    void showAllRecipe();
    void showPartialRecipe(vector<RecipeRow> targetRecipeData);
    vector<Row> stringifyRecipe(vector<RecipeRow> targetRecipeData); // helper function for showRecipe()

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
    vector<RecipeRow> recommendRecipe(string mood);
    bool checkMakable(RecipeRow recipe); // helper function

    vector<IngredientDetail> checkLackIngredient(RecipeRow recipe); // helper function
};

#endif //REFRISYSTEM_REFRIRECIPE_H