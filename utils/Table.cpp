#include <iostream>
#include <vector>
#include <string>
#include "Table.h"

#define MAX_ROW 30

using namespace std;

// Function that finds the longest string length of each column
std::vector<int> findMaxColLengths(const std::vector<std::string>& titles, const std::vector<Row>& rows) {
    std::vector<int> maxColLengths(titles.size(), 0);

    // Find the maximum length from the title
    for (size_t i = 0; i < titles.size(); ++i) {
        maxColLengths[i] = titles[i].size() > MAX_ROW ? MAX_ROW : titles[i].size();
    }

    // Find the maximum length from the rows
    for (const Row& row : rows) {
        for (size_t i = 0; i < row.values.size(); ++i) {
            int currentLength = row.values[i].size() > MAX_ROW ? MAX_ROW : row.values[i].size();
            if (currentLength > maxColLengths[i]) {
                maxColLengths[i] = currentLength;
            }
        }
    }

    return maxColLengths;
}

// Function that draws the line
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

// Function that prints the title or values
void printRow(const std::vector<std::string>& values, const std::vector<int>& colLengths) {
    std::cout << "|";
    for (size_t i = 0; i < values.size(); ++i) {
        std::string value = values[i].size() > MAX_ROW ? values[i].substr(0, MAX_ROW - 3) + "..." : values[i];
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

    vector<int> maxColLengths = findMaxColLengths(titles, rows);

    drawLine(maxColLengths);

    printRow(titles, maxColLengths);

    drawLine(maxColLengths);

    for (const Row& row : rows) {
        printRow(row.values, maxColLengths);
    }

    drawLine(maxColLengths);
}