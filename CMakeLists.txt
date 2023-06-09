cmake_minimum_required(VERSION 3.25)
project(refrisystem)

set(CMAKE_CXX_STANDARD 17)

# json 설정
add_subdirectory(json)

add_executable(refrisystem main.cpp include/Refrigerator.h include/RefriRecipe.h include/RefriSystem.h utils/Table.h src/RefriRecipe.cpp src/RefriSystem.cpp utils/Table.cpp utils/ColorfulCli.h utils/ColorfulCli.cpp utils/CutDecimal.cpp utils/CutDecimal.h utils/Title.cpp utils/Title.h utils/Input.cpp utils/Input.h src/Refrigerator.cpp include/DataManagement.h include/RecipeRecommendation.h src/RecipeRecommendation.cpp src/Recommendation.cpp include/Recommendation.h)
target_link_libraries(refrisystem PRIVATE nlohmann_json::nlohmann_json)

if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    file(WRITE ${PROJECT_BINARY_DIR}\\crun.bat start\ /wait\ ccore.bat)
    file(WRITE ${PROJECT_BINARY_DIR}\\ccore.bat @echo\ off\ntitle\ ${CMAKE_PROJECT_NAME}\ncls\n${CMAKE_PROJECT_NAME}\npause\nexit)
endif()

