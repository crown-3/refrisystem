#ifndef REFRISYSTEM_STORAGE_H
#define REFRISYSTEM_STORAGE_H

#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "DataManagement.h"
#include "Interfaces.h"

using namespace nlohmann;
using namespace std;

class Storage: public DataManagement {
private:
    string RawJSON_path;
    static vector<IngredientItem> ingredientData;
public:
    Storage(string data_path);
    ~Storage();

    void loadData();
    void saveData();
    void addIngredient(IngredientItem ingredient);
    void removeIngredient(string ingredientName, double amount);
    void printStorage();

    // --------------------------------------------------------------

    // clearFreshness 기능 : 멤버 변수 ingredient에서 특정 ingredient name을 찾아 freshness를 초기화
    void clearFreshness(string ingredientName, int init);

    // criteria보다 낮은  freshness를 가진 Ingredients 반환
    vector<IngredientItem> getIngredientsFreshnessLowerThan(int criteria);

    // helper function
    double checkAmount(string ingredientName, int freshThreshold);
};

#endif //REFRISYSTEM_STORAGE_H
