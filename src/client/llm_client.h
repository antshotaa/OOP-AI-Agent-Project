#pragma once

#include <string>
#include <nlohmann/json.hpp>

using namespace std;

class LLMClient {
public:
    // hàm thuẩn ảo, đại khái là để bắt buộc các lớp con kế thừa lớp này phải khai báo và cài đặt 1 hàm chat() riêng của lớp đấy, không khai báo thì lỗi
    virtual nlohmann::json chat(const nlohmann::json& messages) = 0;

    virtual ~LLMClient() = default;
};
