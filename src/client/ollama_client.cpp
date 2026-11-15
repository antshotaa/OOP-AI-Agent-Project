#include <string>
#include <nlohmann/json.hpp>
#include <iostream>
#include <curl/curl.h>
#include "ollama_client.h"
#include "../tools/tool_registry.h"
#include "../tools/calc_tool.h"

using namespace std;

OllamaClient::OllamaClient(
    string url, 
    string m, 
    double temp, 
    int tok
){
    _base_url = url;
    _model = m;
    _temperature = temp;
    _max_tokens = tok;
}
OllamaClient::OllamaClient(){
    _base_url = "http://localhost:11434"; 

    // linh hoạt model mỗi khi chuyển môi trường giữa máy cá nhân và kaggle (vì kaggle là linux)
    #ifdef _WIN32
        _model = "qwen2.5:3b"; 
    #else
        _model = "qwen3.8:27b";
    #endif

    _temperature = 0.7; // số này tùy chỉnh
    _max_tokens = 2048; // tùy
}
string OllamaClient::getUrl(){
    return _base_url;
}
string OllamaClient::getModel(){
    return _model;
}
double OllamaClient::getTemp(){
    return _temperature;
}
int OllamaClient::getTokens(){
    return _max_tokens;
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
    request["model"] = _model;
    request["messages"] = messages;
    request["stream"] = false;
    request["tools"] = tools;
    request["options"]["temperature"] = _temperature;
    request["options"]["num_predict"] = _max_tokens;
    string body = request.dump();

    // bắt đầu gửi request 
    CURL* curl = curl_easy_init();

    if (curl == NULL) {
        return {{"error", "Không thể khởi tạo CURL."}};
    }

    string url = _base_url + "/api/chat";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &answer);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) { // check exception
        cerr << "Request failed: "
            << curl_easy_strerror(res) << endl;

        curl_easy_cleanup(curl);
        return {{"error", curl_easy_strerror(res)}};
    }

    curl_easy_cleanup(curl);

    // đã có câu trả lời từ AI
    try {
        nlohmann::json response = nlohmann::json::parse(answer);
        return response;
    }
    catch (const nlohmann::json::parse_error& e) {
        return {
            {"error", string("JSON parse error: ") + e.what()}
        };
    }
}
