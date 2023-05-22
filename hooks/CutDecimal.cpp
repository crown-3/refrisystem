//
// Created by strik on 2023-05-22.
//
#include <string>
using namespace std;

string CutDecimal(string num, int pos)
{
    return num.substr(0, num.find('.') + pos + 1);
}