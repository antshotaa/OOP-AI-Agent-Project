#include "tool_registry.h"

bool ToolRegistry::registerTool(unique_ptr<Tool> tool){
    if (!tool) return false; 
    for (const unique_ptr<Tool> &t :tools) { // nếu đã có tool rồi thì k thêm nữa
        if (t->name() == tool->name()){
            return false;
        }
    }
    tools.push_back(move(tool));
    return true;
}

Tool* ToolRegistry::getTool(const string& name){
    for (const unique_ptr<Tool> &t :tools){ // lấy tool đơn giản
        if (t->name() == name){
            return t.get();
        }
    }
    return NULL;
}

nlohmann::json ToolRegistry::getToolDefinitions() const{
    nlohmann::json definitions = nlohmann::json::array(); // tạo mảng json rỗng để điền thông tin vào
    for (const unique_ptr<Tool>&t: tools){
        definitions.push_back({
            {"type", "function"},
            {"function", {
                {"name", t->name()},
                {"description", t->description()},
                {"parameters", t->parameters()}
            }}
        });
    }
    return definitions;
}