//
// Created by strik on 2023-05-21.
//

#ifndef REFRISYSTEM_REFRIRECIPE_H
#define REFRISYSTEM_REFRIRECIPE_H

#include <vector>
#include "Table.h"
using namespace std;

class RefriRecipe {
    // vector<Recipe> recipeList;
public:
    RefriRecipe();
    void MAIN_RefriStorage();
    static vector<Row> loadRecipeList(string data_path);
    static void showRecipeList();
    static void removeRecipe(string targetName);
    // ..
};

#endif //REFRISYSTEM_REFRIRECIPE_H
