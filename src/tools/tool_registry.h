#pragma once
#include <tool.h>
#include <calc_tool.h>
class ToolRegistry {
private:
    Calculator calc;
public:
    Tool* getTool(string name){
        if (name == "Calculator"){
            return &calc;
        }
        return NULL;
    }
};