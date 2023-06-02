#ifndef REFRISYSTEM_REFRIRECIPE_H
#define REFRISYSTEM_REFRIRECIPE_H

#include <vector>
#include "../hooks/Table.h"
#include <nlohmann/json.hpp>
using namespace std;
using namespace nlohmann;

// 가능한 RecipeTag 종류
vector<string> RecipeTagList = {
        "Vegan", "Vegetarian",  // 성향 tag
        "Healthy", "Premium",   // 기타 tag
        "Cold", "Hot", "Sweet", "Salty", "Spicy", "Sour", "Savory", // 맛 관련 tag
        "Fat", "Protein", "Carbohydrate", "Vitamin", "Mineral"      // 영양성분 관련 tag
};

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
public:
    // Load from JSON file
    RefriRecipe(string data_path);  // json에서 벡터변수로 parsing해서 load
    void loadRecipeData(); // helper funtion for constructor

    // Save to JSON file
    ~RefriRecipe(); // json에 변경사항 save

    // Show All Recipes
    void showRecipe();
    vector<Row> stringifyRecipe(); // helper function for showRecipe()

    // --------------------------------------------------------------
    // Add Recipe
    void addRecipe(); // 고쳐야 함
    void showPossibleTags(); // helper function for addRecipe()
    vector<IngredientDetail> inputIngredients(); // helper function for addRecipe()
    vector<string> inputSteps(); // helper function for addRecipe()

    // Remove Recipe
    void removeRecipe(); // 고쳐야 함
};

#endif //REFRISYSTEM_REFRIRECIPE_H