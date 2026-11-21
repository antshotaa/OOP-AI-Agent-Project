#pragma once
#include <string>
#include <filesystem>

using namespace std;

namespace fs = std::filesystem;

struct FileResult{
    bool success = true;
    string content = "";
    string error = "";
};

class FileManager{
private:
    fs::path _workspace;
public:
    FileManager(const fs::path &workspace);
    FileResult readFile(const string& path);
    FileResult writeFile(const string& path, const string& content);
};