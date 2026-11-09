#include "loop_detector.h"

bool LoopDetector::isLoop(const string& toolName, const nlohmann::json& args){
    if (!lastCalls.empty()){
        if (args == lastCalls["arguments"] && toolName == lastCalls["name"]) {
            currentLoop++;
            if (currentLoop >= maxLoop){
                return true;
            }
        }
        else {
            currentLoop = 1;
        }
    }
    lastCalls = {
        {"name", toolName},
        {"arguments", args}
    };
    return false;
}