cmake_minimum_required(VERSION 3.25)
project(refrisystem)

set(CMAKE_CXX_STANDARD 17)

# json 설정
add_subdirectory(json)

add_executable(refrisystem main.cpp include/IngredientInfo.h include/Recipe.h include/Food.h include/StorageType.h include/Refrigerator.h include/RefriRecipe.h include/RefriStorage.h include/RefriSystem.h include/Table.h src/IngredientInfo.cpp src/Recipe.cpp src/RefriRecipe.cpp src/RefriSystem.cpp src/Table.cpp src/TimeSystem.cpp src/TimeSystem.cpp include/ColorfulCli.h src/ColorfulCli.cpp)
target_link_libraries(refrisystem PRIVATE nlohmann_json::nlohmann_json)
