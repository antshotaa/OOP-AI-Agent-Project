#pragma once

#include <string>
#include <nlohmann/json.hpp>

using namespace std;

class LLMClient {
public:
    virtual nlohmann::json chat(const nlohmann::json& messages) = 0;
    virtual ~LLMClient() = default;
};
