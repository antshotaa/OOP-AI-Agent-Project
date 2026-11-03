#pragma once

#include <string>
#include <nlohmann/json.hpp>
using namespace std;

class Tool {
private:

public:
    virtual string name() = 0;
    virtual string description() = 0;
    virtual nlohmann::json exec(nlohmann::json& args) = 0;
    virtual ~Tool() = default;
};