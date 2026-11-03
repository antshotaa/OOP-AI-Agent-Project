#pragma once

#include "llm_client.h"
using namespace std;

class OllamaClient : public LLMClient {
private:
    string base_url; // url để kết nối với ollama api
    string model; // model ollama kèm với version
    double temperature; // dùng làm yếu tố ngẫu nhiên trong câu trả lời của AI, hên thì trả lời khôn ko thì trả lời ngu
    int max_tokens; // max_tokens càng nhỏ thì câu trả lời của AI càng ngắn
public: 
    OllamaClient();
    OllamaClient(string url, string m, double temp, int tok);
    nlohmann::json chat(const nlohmann::json& messages) override;
    string getUrl();
    string getModel();
    double getTemp();
    int getTokens();
};
