cmake_minimum_required(VERSION 3.25)
project(refrisystem)

set(CMAKE_CXX_STANDARD 17)

# json 설정
add_subdirectory(json)

add_executable(refrisystem main.cpp include/IngredientInfo.h include/Recipe.h include/Food.h include/StorageType.h include/Refrigerator.h include/RefriRecipe.h include/RefriStorage.h include/RefriSystem.h hooks/Table.h src/IngredientInfo.cpp src/Recipe.cpp src/RefriRecipe.cpp src/RefriSystem.cpp hooks/Table.cpp src/TimeSystem.cpp src/TimeSystem.cpp hooks/ColorfulCli.h hooks/ColorfulCli.cpp hooks/CutDecimal.cpp hooks/CutDecimal.h hooks/Title.cpp hooks/Title.h hooks/Input.cpp hooks/Input.h hooks/TagLists.h)
target_link_libraries(refrisystem PRIVATE nlohmann_json::nlohmann_json)

if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    file(WRITE ${PROJECT_BINARY_DIR}\\crun.bat start\ /wait\ ccore.bat)
    file(WRITE ${PROJECT_BINARY_DIR}\\ccore.bat @echo\ off\ntitle\ ${CMAKE_PROJECT_NAME}\ncls\n${CMAKE_PROJECT_NAME}\npause\nexit)
endif()