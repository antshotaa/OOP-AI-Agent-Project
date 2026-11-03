#include <string>
#include <nlohmann/json.hpp>
#include <iostream>
#include <windows.h>
#include <curl/curl.h>
#include "ollama_client.h"

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
    temperature = 0.7;
    max_tokens = 256;
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
size_t WriteCallback(
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
nlohmann::json OllamaClient::chat(
    const nlohmann::json& messages
){
    string answer;
    nlohmann::json request;
    request["model"] = model;
    request["messages"] = messages;
    request["stream"] = false;
    request["options"]["temperature"] = temperature;
    request["options"]["num_predict"] = max_tokens;

    string body = request.dump();
    CURL* curl = curl_easy_init();

    if (curl == NULL) {
        return "";
    }

    string url = base_url + "/api/chat";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &answer);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        cerr << "Request failed: "
            << curl_easy_strerror(res) << endl;

        curl_easy_cleanup(curl);
        return "";
    }

    curl_easy_cleanup(curl);

    nlohmann::json response = nlohmann::json::parse(answer);
    return response;
}
int main() {
    SetConsoleOutputCP(CP_UTF8); // có 2 dòng này thì mới in ra tiếng việt có dấu dc 
    SetConsoleCP(CP_UTF8);
    cout << "Bắt đầu chương trình..." << endl;
    OllamaClient client;

    cout << "Bắt đầu cuộc trò chuyện..." << endl;
    while (true){

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
            nlohmann::json answer = client.chat(messages);

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