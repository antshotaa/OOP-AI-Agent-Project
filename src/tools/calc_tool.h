#pragma once

#include "tool.h"
using namespace std;

class Calculator : public Tool {
private: 

public:
    string name() override;
    string description() override;
    nlohmann::json exec(nlohmann::json& args) override;
    ~Calculator() = default;
};