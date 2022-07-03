#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

const std::vector<std::string> split(const std::string& str, const char& delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string tok;

    while (std::getline(ss, tok, delimiter)) {
        result.push_back(tok);
    }
    return result;
}