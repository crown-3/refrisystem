#ifndef REFRISYSTEM_REFRIGERATOR_H
#define REFRISYSTEM_REFRIGERATOR_H

#include <string>
#include <vector>
#include "Ingredient.h"
#include "StorageType.h"
#include "nlohmann/json.hpp"

using namespace nlohmann;
using namespace std;

class Refrigerator {
private:
    static vector<Ingredient> ingredient;

public:
    // loadIngredient 기능 : Storage.json 파일의 내용을 멤버 변수 ingredient에 할당
    // Storage.json의 형식 :  [{
    //    "storage": "fridge",
    //    "name": "Avocado",
    //    "freshness": 54,
    //    "amount": 3
    //  }, ... ]
    //
    void loadIngredient();
    // saveIngredient 기능 : 멤버 변수 ingredient를 Storage.json에 저장
    void saveIngredient();
    // addIngredient 기능 : 멤버 변수 ingredient에 인자로 받은 ingredient 추가
    void addIngredient(Ingredient ingredient);
    // removeIngredient 기능 : 멤버 변수 ingredient에서 특정 ingredient의 name을 찾아 amount만큼 삭제
    void removeIngredient(string ingredientName, double amount);

    static vector<Ingredient> getIngredientsFreshnessLowerThan(int criteria);
};

#endif //REFRISYSTEM_REFRIGERATOR_H
