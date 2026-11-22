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

FileResult FileManager::readFile(const string& path) {
    if (path.empty()) {
        return {false, "", "Error: File path is empty."};
    }
    fs::path filePath = fs::absolute(_workspace / path).lexically_normal();
    fs::path workspacePath = fs::absolute(_workspace).lexically_normal();

    auto relativePath = filePath.lexically_relative(workspacePath);
    if (relativePath.empty() ||
        *relativePath.begin() == ".." ||
        relativePath.is_absolute()) {
        return {false, "", "Error: Access outside workspace is not allowed."};
    }
    if (!fs::exists(filePath)) {
        return {false, "", "Error: File does not exist."};
    }
    if (!fs::is_regular_file(filePath)) {
        return {false, "", "Error: Path is not a regular file."};
    }

    const uintmax_t MAX_FILE_SIZE = 10 * 1024 * 1024;

    if (fs::file_size(filePath) > MAX_FILE_SIZE) {
        return {false, "", "Error: File is too large."};
    }
    ifstream file(filePath, ios::binary);
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

FileResult FileManager::writeFile(const string& path, const string& content) {
    if (path.empty()) {
        return {false, "", "Error: File path is empty."};
    }
    fs::path filePath = fs::absolute(_workspace / path).lexically_normal();
    fs::path workspacePath = fs::absolute(_workspace).lexically_normal();

    auto relativePath = filePath.lexically_relative(workspacePath);
    if (relativePath.empty() ||
        *relativePath.begin() == ".." ||
        relativePath.is_absolute()) {
        return {false, "", "Error: Access outside workspace is not allowed."};
    }
    if (fs::exists(filePath) && !fs::is_regular_file(filePath)) {
        return {false, "", "Error: Path is not a regular file."};
    }
    ofstream file(filePath, ios::out | ios::trunc | ios::binary);
    if (!file.is_open()) {
        return {false, "", "Error: Cannot open file for writing."};
    }
    file.write(content.data(), content.size());
    if (file.fail()) {
        return {false, "", "Error: Failed to write contents to file."};
    }
    file.close();
    if (file.fail()) {
        return {false, "", "Error: Failed to close file."};
    }
    return {true, "File written successfully.", ""};
}