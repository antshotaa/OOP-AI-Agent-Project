#include "skill_loader.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

void SkillLoader::loadSkills(const string& directory) { // đọc và nạp skill vào bộ nhớ (cụ thể là vào vector skills)
    skills.clear();
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        cout << "ERROR: Đường dẫn không hợp lệ." << endl;
        return;
    }

    // trong thư mục có đường dẫn directory sẽ có các file .md các skills, mỗi entry là một file đấy
    // sau đó thì duyệt qua từng file để nạp skill
    for (const auto& entry : fs::directory_iterator(directory)) { 
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".md") continue;

        ifstream file(entry.path());
        if (!file.is_open()) continue;

        Skill skill;
        skill.name = entry.path().stem().string();

        string line;
        bool readingContent = false;

        while (getline(file, line)) {
            if (line == "---") {
                readingContent = !readingContent; 
                // mỗi dòng --- phân biệt một loại content khác nhau
                // ta lật biến readingContent để phân biệt 2 loại content trong file .md
                // đó là metadata (name và keywords) và các chỉ dẫn (skill)
                
                continue;
            }

            if (!readingContent) { // đọc metadata trước
                if (line.find("name: ") == 0) {
                    skill.name = line.substr(6);
                }
                else if (line.find("keywords: ") == 0) {
                    stringstream ss(line.substr(10));
                    string keyword;

                    while (getline(ss, keyword, ',')) {
                        skill.keywords.push_back(keyword);
                    }
                }
            }
            else {
                skill.content += line + '\n';
            }
        }

        skills.push_back(skill);
    }
}
