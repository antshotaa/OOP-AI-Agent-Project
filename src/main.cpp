#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#endif

#include "client/ollama_client.h"
#include "tools/tool.h"
#include "tools/tool_registry.h"
#include "tools/calc_tool.h"
#include "agent/agent_loop.h"
using namespace std;

string loadSystemPrompt(const string& filePath) {
    ifstream file(filePath);

    if (!file.is_open()) {
        throw runtime_error("Cannot open system prompt file.");
    }

    stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}


int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // có 2 dòng này thì mới in ra tiếng việt có dấu dc 
    SetConsoleCP(CP_UTF8);
    #endif
    cout << "Bắt đầu chương trình..." << endl;
    // khởi tạo ollama
    OllamaClient client;

    // khởi tạo tool registry
    ToolRegistry registry;
    registry.registerTool(make_unique<Calculator>()); // đăng ký calculator

    // khởi tạo agent loop và history
    AgentLoop agent(client, registry);
    nlohmann::json history = nlohmann::json::array();

    history.push_back({ // dặn dò AI kỹ hơn cho nó làm việc hẳn hoi
        {"role", "system"},
        {"content", loadSystemPrompt("system_prompt.txt")}
    });

    cout << "Bắt đầu cuộc trò chuyện..." << endl;
    while (true){ // vòng lặp trò chuyện với AI chạy đến khi người dùng thoát
        
        cout << "1. Trò chuyên với AI" << endl;
        cout << "2. Thoát" << endl;
        cout << "Lựa chọn của bạn: " << endl;

        string choice;
        getline(cin,choice);
        if (choice == "1"){
            cout << "Viết tin nhắn của bạn: ";

            string question;
            getline(cin,question);

            cout << "Đang gửi tin nhắn..." << endl;

            string answer = agent.run(history,question);

            cout << "Câu trả lời của AI: " << endl;
            cout << answer << endl;
        }

        else if (choice == "2"){
            cout << "Đang thoát chương trình..." << endl;
            return 0;
        }

        else {
            cout << "Lựa chọn không hợp lệ. Mời nhập lại." << endl;
        }
    }

}