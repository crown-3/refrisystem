#include <iostream>
#include <vector>
#include <string>
#include "../include/Table.h"

using namespace std;

// 각 열의 가장 긴 문자열의 길이를 찾는 함수
std::vector<int> findMaxColLengths(const std::vector<std::string>& titles, const std::vector<Row>& rows) {
    std::vector<int> maxColLengths(titles.size(), 0);

    // 타이틀로부터 최대 길이 찾기
    for (size_t i = 0; i < titles.size(); ++i) {
        maxColLengths[i] = titles[i].size() > 40 ? 40 : titles[i].size();
    }

    // 각 열에서 최대 길이 찾기
    for (const Row& row : rows) {
        for (size_t i = 0; i < row.values.size(); ++i) {
            int currentLength = row.values[i].size() > 40 ? 40 : row.values[i].size();
            if (currentLength > maxColLengths[i]) {
                maxColLengths[i] = currentLength;
            }
        }
    }

    return maxColLengths;
}

// 선을 그리는 함수
void drawLine(const std::vector<int>& colLengths) {
    std::cout << "+";
    for (int len : colLengths) {
        for (int i = 0; i < len + 2; ++i) {
            std::cout << "-";
        }
        std::cout << "+";
    }
    std::cout << std::endl;
}

#include <iomanip>

// 제목이나 값을 출력하는 함수
void printRow(const std::vector<std::string>& values, const std::vector<int>& colLengths) {
    std::cout << "|";
    for (size_t i = 0; i < values.size(); ++i) {
        std::string value = values[i].size() > 40 ? values[i].substr(0, 37) + "..." : values[i];
        std::cout << " " << std::fixed << std::setprecision(1) << value;
        for (size_t j = value.size(); j < colLengths[i] + 1; ++j) {
            std::cout << " ";
        }
        std::cout << "|";
    }
    std::cout << std::endl;
}


void table(const std::vector<std::string>& titles, const std::vector<Row>& rows) {
    //example
    /*
     * std::vector<std::string> titles = {"Title1", "Title2", "Title3"};
     *
     * std::vector<Row> rows = {
        {{"Data1", "Data2", "Data3"}},
        {{"Data4", "Data5", "Data6"}},
        {{"Data7", "Data8", "Data9"}}
    };
     * */

    // 각 열의 최대 길이 찾기
    vector<int> maxColLengths = findMaxColLengths(titles, rows);

    // 선 그리기
    drawLine(maxColLengths);

    // 제목 출력
    printRow(titles, maxColLengths);

    // 선 그리기
    drawLine(maxColLengths);

    // 데이터 출력
    for (const Row& row : rows) {
        printRow(row.values, maxColLengths);
    }

    // 마지막 선 그리기
    drawLine(maxColLengths);

}