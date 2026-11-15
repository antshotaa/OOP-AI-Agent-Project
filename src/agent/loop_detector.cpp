#include "loop_detector.h"

bool LoopDetector::isLoop(const string& toolName, const nlohmann::json& args){
    if (!_lastCalls.empty()){
        if (args == _lastCalls["arguments"] && toolName == _lastCalls["name"]) {
            _currentLoop++;
            if (_currentLoop >= _maxLoop){
                return true;
            }
        }
        else {
            _currentLoop = 1;
        }
    }
    _lastCalls = {
        {"name", toolName},
        {"arguments", args}
    };
    return false;
}