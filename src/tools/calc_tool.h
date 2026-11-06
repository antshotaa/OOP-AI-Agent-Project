#pragma once

#include "tool.h"
using namespace std;

class Calculator : public Tool {
private: 

public:
    string name() const override;
    string description() const override;
    nlohmann::json exec(nlohmann::json& args) const override;
    nlohmann::json parameters() const override;
    ~Calculator() = default;
};