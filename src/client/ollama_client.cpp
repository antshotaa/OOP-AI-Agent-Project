#include <string>
#include <nlohmann/json.hpp>
#include <iostream>
#include <windows.h>
#include <curl/curl.h>
#include "ollama_client.h"
#include "../tools/tool_registry.h"
#include "../tools/calc_tool.h"
#include <memory>

using namespace std;

OllamaClient::OllamaClient(
    string url, 
    string m, 
    double temp, 
    int tok
){
    base_url = url;
    model = m;
    temperature = temp;
    max_tokens = tok;
}
OllamaClient::OllamaClient(){
    base_url = "http://localhost:11434"; 
    model = "llama3.2:latest";
    temperature = 0.7; // số này tùy chỉnh
    max_tokens = 256; // tùy
}
string OllamaClient::getUrl(){
    return base_url;
}
string OllamaClient::getModel(){
    return model;
}
double OllamaClient::getTemp(){
    return temperature;
}
int OllamaClient::getTokens(){
    return max_tokens;
}
size_t WriteCallback( // libcurl bắt phải có hàm này để viết response, ko cần hiểu logic lắm đâu
    void* contents,   
    size_t size,
    size_t nmemb,
    void* userp
) {
    size_t totalSize = size * nmemb;

    char* data = static_cast<char*>(contents);
    string* response = static_cast<string*>(userp);

    response->append(data, totalSize);

    return totalSize;
}
nlohmann::json OllamaClient::chat( // nói chuyện với AI
    const nlohmann::json& messages, // lấy tham số là tin nhắn của mình
    const nlohmann::json& tools
){
    string answer;
    nlohmann::json request;

    // setup mẫu request mình cần gửi để AI hiểu dc yêu cầu
    request["model"] = model;
    request["messages"] = messages;
    request["stream"] = false;
    request["tools"] = tools;
    request["options"]["temperature"] = temperature;
    request["options"]["num_predict"] = max_tokens;
    string body = request.dump();

    // bắt đầu gửi request 
    CURL* curl = curl_easy_init();

    if (curl == NULL) {
        return {"error", "Không thể khởi tạo CURL."};
    }

    string url = base_url + "/api/chat";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &answer);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) { // check exception
        cerr << "Request failed: "
            << curl_easy_strerror(res) << endl;

        curl_easy_cleanup(curl);
        return {"error", curl_easy_strerror(res)};
    }

    curl_easy_cleanup(curl);

    // đã có câu trả lời từ AI
    nlohmann::json response = nlohmann::json::parse(answer);
    return response;
}
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