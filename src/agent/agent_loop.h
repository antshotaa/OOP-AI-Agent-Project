#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "../client/llm_client.h"
#include "../tools/tool_registry.h"
#include "skill_loader.h"

using namespace std;
class AgentLoop {
private:
    LLMClient& _client;
    ToolRegistry& _registry;
    int _maxSteps;
    nlohmann::json runTool(const nlohmann::json& call);
    string _systemPrompt;
    SkillLoader& _skillLoader;
public:
    AgentLoop(
        LLMClient& client, 
        ToolRegistry& registry, 
        SkillLoader& skillLoader, 
        const string& systemPrompt, 
        int maxSteps = 5);

    // history có vai trò giữ toàn bộ hội thoại, hàm tự thêm câu hỏi của user vào cuối
    string run(nlohmann::json& history, const string& userInput);
};