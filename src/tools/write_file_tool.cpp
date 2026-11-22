#include "write_file_tool.h"

WriteFileTool::WriteFileTool(FileManager &fileManager) :
    _fileManager(fileManager)
{
    
}

string WriteFileTool::name() const{
    return "write_file";
}

string WriteFileTool::description() const {
    return "Write contents to a file with a given path.";
}

nlohmann::json WriteFileTool::parameters() const{
    return {
        {"type", "object"},
        {"properties", {
            {"path", {
                {"type", "string"},
                {"description", "Path of the file to write"}
            }},
            {"contents", {
                {"type", "string"},
                {"description", "Contents to write in the file"}
            }}
        }},
        {"required", {"path", "contents"}}
    };
}

nlohmann::json WriteFileTool::exec(nlohmann::json &args){
    nlohmann::json result = {
        {"success", true},
        {"content", ""},
        {"error", ""}
    };
    string path = args.at("path");
    string contents = args.at("contents");
    FileResult fileResult = _fileManager.writeFile(path, contents);
    result["success"] = fileResult.success;
    result["content"] = fileResult.content;
    result["error"] = fileResult.error;
    return result;
}