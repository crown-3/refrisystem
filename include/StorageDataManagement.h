#ifndef REFRISYSTEM_STORAGEDATAMANAGEMENT_H
#define REFRISYSTEM_STORAGEDATAMANAGEMENT_H

#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "DataManagement.h"
#include "Interfaces.h"

using namespace nlohmann;
using namespace std;

class StorageDataManagement: public DataManagement {
private:
    string RawJSON_path;
    vector<IngredientItem> data;
public:
    StorageDataManagement(string data_path);
    ~StorageDataManagement();

    void loadData() override;
    void saveData() override;

    vector<IngredientItem> getData();
    void addData(IngredientItem ingredient);
    void removeData(string ingredientName, double amount);
    void clearFreshness(string ingredientName, int init, int criteria);
};

#endif //REFRISYSTEM_STORAGEDATAMANAGEMENT_H
