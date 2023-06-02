//
// Created by strik on 2023-05-21.
//

#ifndef REFRISYSTEM_RECIPE_H
#define REFRISYSTEM_RECIPE_H

#include <vector>
#include <map>
#include "Ingredient.h"

class Recipe {
    string name;
    map<Ingredient, double> ingredients;
    vector<string> tags;
public:
    // 파일 입출력 관리
    static vector<vector<string>> inputData(string path);

    // show all current recipes
    void showRecipe();

    // 추가해야할 기능
//    bool canBeMade(Refrigerator ref);
//    map<Food, double> getNeededIngredients(Refrigerator);
};

#endif //REFRISYSTEM_RECIPE_H
