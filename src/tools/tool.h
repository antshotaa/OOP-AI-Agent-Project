#pragma once
#include <string>
#include <nlohmann/json.hpp>
using namespace std;
class Tool {
public:
    virtual string name() const = 0; // tên tools
    virtual string description() const = 0; // mô tả tools
    virtual nlohmann::json exec(nlohmann::json& args) const = 0; // thực thi tools
    virtual nlohmann::json parameters() const = 0; // tham số bổ sung, 
    // ví dụ calculator sẽ cần tham số expression

    virtual ~Tool() = default;
};