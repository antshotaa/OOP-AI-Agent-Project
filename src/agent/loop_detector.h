#pragma once

#include <string>
#include <nlohmann/json.hpp>
using namespace std;

class LoopDetector{
private:
    int _currentLoop = 0;
    int _maxLoop = 5;
    nlohmann::json _lastCalls = nlohmann::json::array();
public:
    bool isLoop(const string& toolName, const nlohmann::json& args);
};