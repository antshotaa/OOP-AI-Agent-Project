#include "read_file_tool.h"

ReadFileTool::ReadFileTool(FileManager &fileManager) :
    _fileManager(fileManager)
{
    
}

string ReadFileTool::name() const{
    return "read_file";
}

string ReadFileTool::description() const {
    return "Read the content of a file from the given path.";
}

nlohmann::json ReadFileTool::parameters() const{
    return {
        {"type", "object"},
        {"properties", {
            {"path", {
                {"type", "string"},
                {"description", "Path of the file to read"}
            }}
        }},
        {"required", {"path"}}
    };
}

nlohmann::json ReadFileTool::exec(nlohmann::json &args){
    nlohmann::json result = {
        {"success", true},
        {"content", ""},
        {"error", ""}
    };
    string path = args.at("path");
    FileResult fileResult = _fileManager.readFile(path);
    result["success"] = fileResult.success;
    result["content"] = fileResult.content;
    result["error"] = fileResult.error;
    return result;
}