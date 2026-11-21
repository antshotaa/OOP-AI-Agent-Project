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
                {"description", "Path of the file write"}
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
    
}