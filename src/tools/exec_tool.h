#pragma once

#include "tool.h"
#include <string>
#include <nlohmann/json.hpp>

using namespace std;

class ExecTool : public Tool {
private:
    bool enableSafetyCheck;
    bool isDangerousCommand(const string& cmd) const;

public:
    explicit ExecTool(bool safetyCheck = true);

    string name() const override;
    string description() const override;
    nlohmann::json exec(nlohmann::json& args) override;
    nlohmann::json parameters() const override;

    ~ExecTool() override = default;
};
