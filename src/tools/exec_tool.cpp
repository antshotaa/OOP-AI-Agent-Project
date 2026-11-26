#include "exec_tool.h"
#include <cstdio>
#include <memory>
#include <array>
#include <vector>

#ifdef _WIN32
    #define POPEN _popen
    #define PCLOSE _pclose
#else
    #define POPEN popen
    #define PCLOSE pclose
#endif

using namespace std;

ExecTool::ExecTool(bool safetyCheck) : enableSafetyCheck(safetyCheck) {}

string ExecTool::name() const {
    return "exec";
}

string ExecTool::description() const {
    return "Execute a shell/terminal command on the host computer and return its output (both stdout and stderr). "
           "Use this tool when you need to run scripts, check files/directories, compile code, or inspect system information.";
}

nlohmann::json ExecTool::parameters() const {
    return {
        {"type", "object"},
        {"properties", {
            {"command", {
                {"type", "string"},
                {"description", "The shell command to execute (e.g., 'dir', 'ls -la', 'python script.py')"}
            }}
        }},
        {"required", {"command"}}
    };
}

bool ExecTool::isDangerousCommand(const string& cmd) const {
    // Danh sách các từ khóa lệnh nguy hiểm tránh việc AI tự ý xóa dữ liệu hoặc tắt máy
    const vector<string> blacklist = {
        "rm -rf /", "rm -rf *", "mkfs", "dd if=", "format ", "shutdown", "reboot", "del /f /s /q c:\\"
    };
    for (const auto& bad : blacklist) {
        if (cmd.find(bad) != string::npos) {
            return true;
        }
    }
    return false;
}

nlohmann::json ExecTool::exec(nlohmann::json& args) {
    // 1. Kiểm tra tham số đầu vào từ AI
    if (!args.contains("command") || !args["command"].is_string()) {
        return {{"error", "Thiếu tham số 'command' hoặc kiểu dữ liệu không phải chuỗi."}};
    }

    string command = args.at("command").get<string>();
    if (command.empty()) {
        return {{"error", "Câu lệnh 'command' không được để trống."}};
    }

    // 2. Kiểm tra an toàn trước khi chạy
    if (enableSafetyCheck && isDangerousCommand(command)) {
        return {{"error", "Từ chối thực thi: Lệnh nằm trong danh sách nguy hiểm bị chặn."}};
    }

    // 3. Nối " 2>&1" để gộp cả thông báo lỗi (stderr) vào kết quả trả về (stdout)
    string fullCommand = command + " 2>&1";
    int exitCode = 0;

    // Dùng unique_ptr với custom deleter (C++17) để tự động đóng pipe an toàn, tránh rò rỉ bộ nhớ
    auto pipeDeleter = [&exitCode](FILE* fp) {
        if (fp != nullptr) {
            int rawStatus = PCLOSE(fp);
            #ifdef _WIN32
                exitCode = rawStatus;
            #else
                exitCode = WIFEXITED(rawStatus) ? WEXITSTATUS(rawStatus) : rawStatus;
            #endif
        }
    };

    string output;
    array<char, 256> buffer{};

    {
        unique_ptr<FILE, decltype(pipeDeleter)> pipe(POPEN(fullCommand.c_str(), "r"), pipeDeleter);
        if (!pipe) {
            return {{"error", "Lỗi hệ thống: Không thể mở tiến trình chạy lệnh."}};
        }

        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
            output += buffer.data();
        }
    } // <-- Pipe tự động đóng tại đây và cập nhật biến exitCode

    // 4. Trả về JSON đúng chuẩn format giống Calculator của nhóm
    return {
        {"message", "Thực thi lệnh thành công."},
        {"exit_code", exitCode},
        {"output", output.empty() ? "(Lệnh chạy xong, không có nội dung in ra màn hình)" : output}
    };
}
