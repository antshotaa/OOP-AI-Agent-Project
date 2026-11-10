#include "agent_loop.h"
#include "loop_detector.h"
#include <iostream>

AgentLoop::AgentLoop(LLMClient& client, ToolRegistry& registry, int maxSteps) 
        : client(client), registry(registry), maxSteps(maxSteps)
{

} 

string AgentLoop::run(nlohmann::json& history, const string& userInput){ 
    history.push_back({ // đưa dữ liệu người dùng nhập vào vào lịch sử trò chuyện
        {"role", "user"},
        {"content", userInput}
    });
    LoopDetector loopDetector;
    for (int step = 0; step < maxSteps; step++){ // mỗi step là 1 lần gọi llm
        nlohmann::json tools = registry.getToolDefinitions();

        nlohmann::json response = client.chat(history,tools);

        cout << "DEBUG response: " << response.dump(4) << endl; // dùng cho việc debug

        if (response.contains("message") && 
            response["message"].contains("tool_calls") && 
            !response["message"]["tool_calls"].empty()) 
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

                if (loopDetector.isLoop(toolName,arguments)){ // check vòng lặp vô hạn
                    return "Phát hiện có vòng lặp. Ngừng thực thi.";
                }
                // chạy tools
                Tool* tool = registry.getTool(toolName);

                nlohmann::json result;

                if (tool) result = tool->exec(arguments);
                else {
                    return "Không tìm thấy tool: " + toolName;
                }
                // cout << "DEBUG tool result: " << result.dump(4) << endl;
                history.push_back({
                    {"role", "tool"},
                    {"tool_name", toolName},
                    {"content", result.dump()}
                });
            }
        }
        else {
            if (!response.contains("message") ||
                !response["message"].is_object()) {
                return "Phản hồi từ LLM không hợp lệ.";
            }
            cout << "DEBUG message: " << response["message"].dump(4) << endl; // dùng cho việc debug
            return response["message"]["content"];
        }
    }
    return "Agent đã đạt giới hạn số bước";
}