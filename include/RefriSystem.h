//
// Created by strik on 2023-05-21.
//

#ifndef REFRISYSTEM_REFRISYSTEM_H
#define REFRISYSTEM_REFRISYSTEM_H

#include "RefriStorage.h"
#include "RefriRecipe.h"

class RefriSystem{
    int select; // 메뉴 선택 상태 (1~5)
    bool flag = true; // 상세 페이지 IN 또는 OUT
    // bool is_ON = true; // 냉장고 ON 또는 OFF

    RefriStorage refriStorage;
    RefriRecipe refriRecipe;

public:
    void MAIN_MENU();
};

//class RefriSystem {
//    Refrigerator fridge;
//    vector<Recipe> recipes;
//    // ShoppingList shoppingList;
//    // Cart cart;
//public:
//    void addRecipe(Recipe);
//    void removeRecipe(string);
//    void makeRecipe(string);
//    // void viewShoppingList();
//    // void addToCart(Food, double);
//    // void checkoutCart();
//};

#endif //REFRISYSTEM_REFRISYSTEM_H
