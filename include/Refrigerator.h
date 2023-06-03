#ifndef REFRISYSTEM_REFRIGERATOR_H
#define REFRISYSTEM_REFRIGERATOR_H

#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using namespace nlohmann;
using namespace std;

// enum StorageType
enum StorageType {
    FRIDGE,
    FREEZER,
    PANTRY
};

// Ingredient 틀
struct Ingredient {
    int id;
    string name;
    double quantity;
    int freshness;
    StorageType storageType;
};

// Refrigerator 클래스
class Refrigerator {
private:
    string RawJSON_path;
    static vector<Ingredient> ingredientData;
public:
    // Load from JSON file
    /*
     * loadIngredient 기능 : Storage.json 파일의 내용을 멤버 변수 ingredient에 할당
     * Storage.json의 형식 :  [{
        "storage": "fridge",
        "name": "Avocado",
        "freshness": 54,
        "amount": 3
     * }, ... ]
     */
    Refrigerator(string data_path);
    void loadIngredient();

    // Save to JSON file  (멤버 변수 ingredient를 Storage.json에 저장)
    ~Refrigerator();
    void saveIngredient();

    // --------------------------------------------------------------
    // addIngredient 기능 : 멤버 변수 ingredient에 인자로 받은 ingredient 추가
    void addIngredient(Ingredient ingredient);

    // removeIngredient 기능 : 멤버 변수 ingredient에서 특정 ingredient의 name을 찾아 amount만큼 삭제
    void removeIngredient(string ingredientName, double amount);

    // clearFreshness 기능 : 멤버 변수 ingredient에서 특정 ingredient name을 찾아 freshness를 초기화
    void clearFreshness(string ingredientName, int init);

    // criteria보다 낮은  freshness를 가진 Ingredients 반환
    vector<Ingredient> getIngredientsFreshnessLowerThan(int criteria);

    // 현재 ingredientData를 Table로 출력
    void printStorage();

    // helper function
    double checkAmount(string ingredientName, int freshThreshold);
};

#endif //REFRISYSTEM_REFRIGERATOR_H
