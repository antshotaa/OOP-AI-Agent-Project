#pragma once

#include <string>
#include <nlohmann/json.hpp>
using namespace std;

class LoopDetector{
private:
    int currentLoop = 0;
    int maxLoop = 5;
    nlohmann::json lastCalls = nlohmann::json::array();
public:
    bool isLoop(const string& toolName, const nlohmann::json& args);
};