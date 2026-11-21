#pragma once
#include "tool.h"
#include "file_manager.h"
#include <nlohmann/json.hpp>

class WriteFileTool : public Tool {
private:
    FileManager& _fileManager;

public:
    WriteFileTool(FileManager& fileManager);
    string name() const override;
    string description() const override;
    nlohmann::json parameters() const override;
    nlohmann::json exec(nlohmann::json& args) override;
};