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
    vector<IngredientItem> ingredientData;
public:
    StorageDataManagement(string data_path);
    ~StorageDataManagement();

    void loadData();
    void saveData();
    void addData(IngredientItem ingredient);
    void removeData(string ingredientName, double amount);
};

#endif //REFRISYSTEM_STORAGEDATAMANAGEMENT_H
