#ifndef REFRISYSTEM_REFRIGERATOR_H
#define REFRISYSTEM_REFRIGERATOR_H

#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "DataManagement.h"

using namespace nlohmann;
using namespace std;

// StorageType
enum StorageType {
    FRIDGE,
    FREEZER,
    PANTRY
};

// Ingredient
struct Ingredient {
    int id;
    string name;
    double quantity;
    int freshness;
    StorageType storageType;
};

class Refrigerator: public DataManagement {
private:
    string RawJSON_path;
    static vector<Ingredient> ingredientData;
public:
    Refrigerator(string data_path);
    ~Refrigerator();

    void loadData();
    void saveData();
    void addIngredient(Ingredient ingredient);
    void removeIngredient(string ingredientName, double amount);
    void printStorage();

    // --------------------------------------------------------------

    // clearFreshness 기능 : 멤버 변수 ingredient에서 특정 ingredient name을 찾아 freshness를 초기화
    void clearFreshness(string ingredientName, int init);

    // criteria보다 낮은  freshness를 가진 Ingredients 반환
    vector<Ingredient> getIngredientsFreshnessLowerThan(int criteria);

    // helper function
    double checkAmount(string ingredientName, int freshThreshold);
};

#endif //REFRISYSTEM_REFRIGERATOR_H
