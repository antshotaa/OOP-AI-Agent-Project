#pragma once
#include "tool.h"
#include "file_manager.h"
#include <nlohmann/json.hpp>

class ReadFileTool : public Tool {
private:
    FileManager& _fileManager;

public:
    ReadFileTool(FileManager& fileManager);
    string name() const override;
    string description() const override;
    nlohmann::json parameters() const override;
    nlohmann::json exec(nlohmann::json& args) override;
};