#include <iostream>
#include "include/RefriSystem.h"

#include <fstream>
#include <sstream>
#include "include/Recipe.h"
#include "hooks/Table.h"

#include "include/RefriRecipe.h"

using namespace std;

int main(void){
//    RefriSystem refri;
//    refri.MAIN_MENU();

    RefriRecipe::showRecipeList();
    //RefriRecipe::addRecipe();
    RefriRecipe::removeRecipe();

    return 0;
}
