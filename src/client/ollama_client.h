#pragma once

#include "llm_client.h"
using namespace std;

class OllamaClient : public LLMClient {
private:
    string base_url; 
    string model;
    double temperature;
    int max_tokens;
public: 
    OllamaClient();
    OllamaClient(string url, string m, double temp, int tok);
    nlohmann::json chat(const nlohmann::json& messages) override;
    string getUrl();
    string getModel();
    double getTemp();
    int getTokens();
};
