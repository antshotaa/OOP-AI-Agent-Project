#include "agent_loop.h"
#include "loop_detector.h"
#include <exception>
#include <iostream>

AgentLoop::AgentLoop(
    LLMClient& client,
    ToolRegistry& registry, 
    SkillLoader& skillLoader, 
    const string &systemPrompt,
    int maxSteps) :

    _client(client), 
    _registry(registry), 
    _maxSteps(maxSteps), 
    _skillLoader(skillLoader),
    _systemPrompt(systemPrompt)
{

} 

string AgentLoop::run(nlohmann::json& history, const string& userInput){ 
    history.push_back({ // đưa dữ liệu người dùng nhập vào vào lịch sử trò chuyện
        {"role", "user"},
        {"content", userInput}
    });

    // nhập system prompt vào đầu tiên, 
    // làm thế này để trả history về trạng thái ban đầu đó là chỉ có system prompt
    // nhằm xóa đi skill trước đó đã nhập vào
    history[0]["content"] = _systemPrompt; 
    const Skill* skill = _skillLoader.selectSkill(userInput); 

    if (skill != nullptr) {
        // sau đó là nhập skill cần thiết vào
        history[0]["content"] += skill->content;
    }
    cout << "DEBUG active skill: " << (skill ? skill->name : "None") << endl;
    LoopDetector loopDetector;
    for (int step = 0; step < _maxSteps; step++){ // mỗi step là 1 lần gọi llm
        nlohmann::json tools = _registry.getToolDefinitions();

        nlohmann::json response = _client.chat(history,tools);
        if (response.is_object() && response.contains("error")) {
            if (response["error"].is_string()) {
                return "Lỗi từ Ollama: " + response["error"];
            }
            return "Ollama trả về lỗi không hợp lệ.";
        }
        if (!response.is_object() || !response.contains("message") ||
            !response["message"].is_object()) {
            return "Phản hồi từ LLM không hợp lệ.";
        }
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
                // check điều kiện gọi tool thôi, không cần để ý lắm
                if (!calls[call_number].is_object() ||
                    !calls[call_number].contains("function") ||
                    !calls[call_number]["function"].is_object()) {
                    return "Cấu trúc tool call không hợp lệ.";
                }
                const nlohmann::json& function = calls[call_number]["function"];

                if (!function.contains("name") ||
                    !function["name"].is_string() ||
                    !function.contains("arguments") ||
                    !function["arguments"].is_object()) {
                    return "Tên tool hoặc arguments không hợp lệ.";
                }
                // lấy tên và tham số cần thiết để thực thi tools
                string toolName = function["name"];
                nlohmann::json arguments = function["arguments"];

                if (loopDetector.isLoop(toolName,arguments)){ // check vòng lặp vô hạn
                    return "Phát hiện có vòng lặp. Ngừng thực thi.";
                }
                // chạy tools
                Tool* tool = _registry.getTool(toolName);

                nlohmann::json result;

                if (tool) {
                    try {
                        result = tool->exec(arguments);
                    }
                    catch (const exception& e) {
                        result = {
                            {"error", e.what()}
                        };
                    }
                    catch (...) {
                        result = {
                            {"error", "Unknown exception occurred."}
                        };
                    }
                }
                else {
                    result = {
                        {"error", "Không tìm thấy tool: " + toolName}
                    };
                }
                // cout << "DEBUG tool result: " << result.dump(4) << endl;
                history.push_back({
                    {"role", "tool"},
                    {"tool_name", toolName},
                    {"content", result.dump()}
                });
                if (result.is_object() && result.contains("error")){
                    const Skill* recovery = _skillLoader.findSkill("Error Recovery");
                    if (recovery != nullptr){
                        history[0]["content"] = history[0]["content"].get<string>() + "\n\nActive Skill: Error Recovery\n" + recovery->content;
                        cout << "DEBUG: Error Recovery activated." << endl;
                    }
                    break;
                }
            }
        }
        else {
            if (!response.contains("message") ||
                !response["message"].is_object()) {
                return "Phản hồi từ LLM không hợp lệ.";
            }
            cout << "DEBUG message: " << response["message"].dump(4) << endl; // dùng cho việc debug
            if (!response["message"].contains("content") ||
                !response["message"]["content"].is_string()) {
                return "Phản hồi từ LLM không có nội dung hợp lệ.";
            }
            return response["message"]["content"];
        }
    }
    // hết số bước thực thi tool, gọi LLM thêm một lần để tổng hợp
    nlohmann::json finalResponse =
        _client.chat(history, nlohmann::json::array());
    if (finalResponse.is_object() && finalResponse.contains("error")) {
        if (finalResponse["error"].is_string()) {
            return "Lỗi từ Ollama: " + finalResponse["error"];
        }
        return "Ollama trả về lỗi không hợp lệ.";
    }
    cout << "DEBUG final response: " << finalResponse.dump(4) << endl;

    if (!finalResponse.is_object() ||
        !finalResponse.contains("message") ||
        !finalResponse["message"].is_object()) {
        return "Phản hồi cuối cùng từ LLM không hợp lệ.";
    }

    if (!finalResponse["message"].contains("content") ||
        !finalResponse["message"]["content"].is_string()) {
        return "LLM không trả về câu trả lời cuối cùng hợp lệ.";
    }

    return finalResponse["message"]["content"].get<string>();
}