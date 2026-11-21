#include "file_manager.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <exception>

namespace fs = std::filesystem;

FileManager::FileManager(const fs::path& workspace) :
    _workspace(workspace)
{

}

FileResult FileManager::readFile(const string& path){
    if (path.empty()) {
        return {false, "", "Error: File path is empty."};
    }
    if (!fs::exists(path)) {
        return {false, "", "Error: File does not exist."};
    }
    if (!fs::is_regular_file(path)) {
        return {false, "", "Error: Path is not a regular file."};
    }
    const uintmax_t MAX_FILE_SIZE = 10 * 1024 * 1024;
    if (fs::file_size(path) > MAX_FILE_SIZE) {
        return {false, "", "Error: File is too large."};
    }
    ifstream file(path, ios::binary);
    if (!file.is_open()) {
        return {false, "", "Error: Cannot open file. Permission denied or file is unavailable."};
    }
    string content(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );
    if (file.bad()) {
        return {false, "", "Error: Failed to read file."};
    }
    file.close();
    if (file.fail()) {
        return {false, "", "Error: Failed to close file."};
    }
    
    return {true, content, ""};
}

FileResult FileManager::writeFile(const string& path, const string& content){
    
}