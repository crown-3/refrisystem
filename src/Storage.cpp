#include "../include/Storage.h"
#include <vector>
#include "../hooks/Table.h"
#include <nlohmann/json.hpp>
#include <fstream>

using namespace std;
using namespace nlohmann;

vector<Row> Storage::loadStorage() {
    // Example output
    /*
     * {{"0", "Fridge", "Pineapple", "30", "0.4", "Yes"}}
     * {{"1", "Pantry", "Banana", "0 (ROTTEN)", "2", "Yes"}}
     * {{"2", "Freezer","Pizza", "2 (DANGER)", "2", "YES"}}
     * ...
     * */
    vector<Row> result;

    // Load Storage.json
    ifstream storageFile("Storage.json");
    json storageJson;
    storageFile >> storageJson;

    // Load FoodData.json
    ifstream foodDataFile("FoodData.json");
    json foodDataJson;
    foodDataFile >> foodDataJson;


    // Convert FoodData.json to a more searchable format
    map<string, string> foodDataMap;
    for (auto &element : foodDataJson) {
        foodDataMap[element["name"]] = element["storage"];
    }

    // Loop through each food item in Storage.json
    int index = 0;
    for (auto &element : storageJson) {
        Row row;
        string freshnessIndicator = "";

        int freshness = element["freshness"];
        if (freshness <= 0) {
            freshnessIndicator = " (ROTTEN)";
        } else if (freshness <= 10) {
            freshnessIndicator = " (DANGER)";
        }

        string storage = element["storage"];
        string foodName = element["name"];
        string suitStorage = foodDataMap[foodName] == storage ? "Yes" : "No";

        row.values.push_back(to_string(index++));
        row.values.push_back(storage);
        row.values.push_back(foodName);
        row.values.push_back(to_string(freshness) + freshnessIndicator);
        row.values.push_back(to_string(element["amount"]));
        row.values.push_back(suitStorage);

        result.push_back(row);
    }

    return result;
}

void Storage::showStorage() {

}

void Storage::removeItem() {}

void Storage::depositAllRotten() {}

void Storage::sortItems() {}