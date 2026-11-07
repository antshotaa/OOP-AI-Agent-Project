#include "agent_loop.h"

AgentLoop::AgentLoop(LLMClient& client, ToolRegistry& registry, int maxSteps) 
        : client(client), registry(registry), maxSteps(maxSteps)
{

} 

string AgentLoop::run(nlohmann::json& history, const string& userInput){ 
    history.push_back({ // đưa dữ liệu người dùng nhập vào vào lịch sử trò chuyện
        {"role", "user"},
        {"content", userInput}
    });
    for (int step = 0; step < maxSteps; step++){ // các step bao gồm thực thi tool, trả về kết quả cho user
        nlohmann::json tools = registry.getToolDefinitions();

        nlohmann::json response = client.chat(history,tools);

        if (response["messages"].contains("tool_calls") && !response["message"]["tool_calls"].empty()) 
        // check nếu có gọi tools thì sẽ thực thi tools
        {
            history.push_back(response["message"]); // thêm phản hồi của AI vào history trước

            nlohmann::json calls = response["message"]["tool_calls"];
            

            for (int call_number = 0; call_number < calls.size(); call_number++)
            // có thể có nhiều tool calls trong cùng 1 user input, nên phải duyệt từng cái
            // chẳng hạn người dùng có thể hỏi 66*37 và 1907 + 2262 bằng mấy trong cùng 1 câu
            {
                // lấy tên và tham số cần thiết để thực thi tools
                string toolName = calls[call_number]["function"]["name"];
                nlohmann::json arguments = calls[call_number]["function"]["arguments"];

                // chạy tools
                Tool* tool = registry.getTool(toolName);
                nlohmann::json result;
                if (tool) result = tool->exec(arguments);
                history.push_back({
                    {"role", "tool"},
                    {"content", result.dump()}
                });
            }
        }
        else {
            return response["message"]["content"];
        }
    }
    return "Agent đã đạt giới hạn số bước";
}