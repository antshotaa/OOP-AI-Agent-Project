#pragma once
#include "tool.h"
#include <memory>
#include <vector>
class ToolRegistry {
private:
    vector<unique_ptr<Tool>> tools;
public:
    bool registerTool(unique_ptr<Tool> tool);
    Tool* getTool(const string& name);
    nlohmann::json getToolDefinitions() const;
};