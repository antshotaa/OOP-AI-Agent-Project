#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "../client/llm_client.h"
#include "../tools/tool_registry.h"

using namespace std;
class AgentLoop {
private:
    LLMClient& client;
    ToolRegistry& registry;
    int maxSteps;
    nlohmann::json runTool(const nlohmann::json& call);
public:
    AgentLoop(LLMClient& client, ToolRegistry& registry, int maxSteps = 5);

    // history có vai trò giữ toàn bộ hội thoại, hàm tự thêm câu hỏi của user vào cuối
    string run(nlohmann::json& history, const string& userInput);
};