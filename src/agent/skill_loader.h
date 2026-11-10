#pragma once

#include <string>
#include <vector>

using namespace std;

struct Skill {
    string name;
    vector<string> keywords;
    string content;
};

class SkillLoader {
private:
    vector<Skill> skills;

public:
    void loadSkills(const string& directory);
    const Skill* selectSkill(const string& task) const;
};