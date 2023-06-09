//
// Created by strik on 2023-05-21.
//

#ifndef REFRISYSTEM_TABLE_H
#define REFRISYSTEM_TABLE_H

#include <vector>
#include <string>
using namespace std;

// 각 행을 표현하는 구조체
struct Row {
    vector<string> values;
};

// 테이블을 출력하는 함수
void table(const vector<string>& titles, const vector<Row>& rows);

#endif //REFRISYSTEM_TABLE_H
