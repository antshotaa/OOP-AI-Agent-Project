#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "client/ollama_client.h"
#include "tools/tool.h"
#include "tools/tool_registry.h"
#include "tools/calc_tool.h"
using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8); // có 2 dòng này thì mới in ra tiếng việt có dấu dc 
    SetConsoleCP(CP_UTF8);
    cout << "Bắt đầu chương trình..." << endl;
    OllamaClient client;

    cout << "Bắt đầu cuộc trò chuyện..." << endl;
    while (true){ // vòng lặp trò chuyện với AI chạy đến khi người dùng thoát
        // à mà tuy nói là chat đến khi thoát ra nhưng AI hiện tại chưa biết ghi nhớ
        // tức là hỏi câu nào thì trả lời mỗi câu đấy thôi xong nó quên sạch mọi thứ
        
        cout << "1. Trò chuyên với AI" << endl;
        cout << "2. Thoát" << endl;
        cout << "Lựa chọn của bạn: " << endl;

        string choice;
        getline(cin,choice);
        if (choice == "1"){
            cout << "Viết tin nhắn của bạn: ";

            string question;
            getline(cin,question);
            nlohmann::json messages = nlohmann::json::array();
            messages.push_back({
                {"role", "user"},
                {"content", question}
            });

            cout << "Đang gửi tin nhắn..." << endl;
            ToolRegistry registry;
            registry.registerTool(make_unique<Calculator>());
            nlohmann::json tools = registry.getToolDefinitions();

            nlohmann::json answer = client.chat(messages,tools);

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