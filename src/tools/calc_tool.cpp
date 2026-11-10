#include "calc_tool.h"
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

#define MAX_SIZE 1000
using namespace std;

string Calculator::name() const{
    return "Calculator";
}

string Calculator::description() const{
    return "Evaluate mathematical expressions involving numbers and arithmetic operators. "
           "Use this tool only when the user explicitly asks to calculate or solve a mathematical expression. "
           "NEVER use this tool for general questions, factual questions, or questions that do not require calculation.";
}

bool isValidExpression(const string& s) {
    bool expectNumber = true;
    bool hasNumber = false;
    bool negative = false;

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == ' ') continue;
        if (expectNumber) {
            if (c == '-' && !negative) {
                negative = true;
                continue;
            }
            if (c < '0' || c > '9') return false;
            while (i + 1 < s.size() &&
                   s[i + 1] >= '0' &&
                   s[i + 1] <= '9') {
                i++;
            }

            expectNumber = false;
            negative = false;
            hasNumber = true;
        }
        else {
            if (c != '+' && c != '-' &&
                c != '*' && c != '/') {
                return false;
            }
            expectNumber = true;
        }
    }
    return hasNumber && !expectNumber;
}

nlohmann::json Calculator::exec(nlohmann::json& args) { // thuc hien tinh toan
    string expression = args.at("expression").get<string>();
    if (expression.empty()){
        return {{"error","Biểu thức rỗng."}};
    }
    if (!isValidExpression(expression)){
        return {{"error","Biểu thức không hợp lệ."}};
    }
    int n = expression.size();
    long long nums[MAX_SIZE];
    char ops[MAX_SIZE];
    int p1 = 0; 
    int p2 = 0;
    int sign = 1;
    string cur;
    for (int i = n - 1; i >= 0; i--){
        if (expression[i] == ' ') {
            expression.pop_back();
            n--;
            continue;
        }
        else break;
    }
    for (int i = 0; i < n; i++){
        char c = expression[i];
        if (c == ' ') continue;

        if (c >= '0' && c <= '9') cur.push_back(c);
        if (
            c == '+' ||
            c == '-' || 
            c == '*' || 
            c == '/' || 
            i == n - 1
        ){
            if (cur.empty()){
                if (c == '*' || c == '/') return {{"error", "Biểu thức không hợp lệ."}};
                if (c == '-'){
                    if (sign == -1) sign = 1;
                    else sign = -1;
                }
            }
            else {
                long long add = sign*stoll(cur);
                sign = 1;
                if (p1 == MAX_SIZE-1){
                    return {{"error","Biểu thức quá dài."}};
                }
                nums[p1++] = add;
                cur.clear();
                if (i < n - 1) ops[p2++] = c;
            }
        }
        else if (c < '0' || c > '9'){
            return {{"error", "Biểu thức không hợp lệ."}};
        }
    }
    long long result = 0;
    for (int i = 0; i < p2; i++){
        if (ops[i] == '*') {
            nums[i+1] = nums[i]*nums[i+1];
            nums[i] = 0;
            if (i > 0 && ops[i-1] == '-'){
                ops[i] = '-';
            }
            else ops[i] = '+';
        }
        if (ops[i] == '/'){
            if (nums[i+1] == 0){
                return {{"error","Không thể chia cho 0."}};
            }
            nums[i+1] = nums[i]/nums[i+1];
            nums[i] = 0;
            if (i > 0 && ops[i-1] == '-'){
                ops[i] = '-';
            }
            else ops[i] = '+';
        }
    }
    // for (int i = 0; i < p1; i++) {
    //     cout << "nums[" << i << "] = " << nums[i] << endl;
    // }
    if (p1 > 0) result += nums[0];
    for (int i = 0; i < p1 - 1; i++){
        if (ops[i] == '+'){
            result += nums[i+1];
        }
        else result -= nums[i+1];
    }
    return {
        {"message", "Kết quả là:"},
        {"result", result}
    };
}

nlohmann::json Calculator::parameters() const {
    return {
        {"type", "object"},
        {"properties", {
            {"expression", {
                {"type", "string"},
                {"description", "Mathematical expression to calculate"}
            }}
        }},
        {"required", {"expression"}}
    };
}

// int main() { // hàm main này để test calculator
//     Calculator calc;

//     nlohmann::json args = {
//         {"expression", " "}
//     };

//     nlohmann::json result = calc.exec(args);

//     cout << result.dump(4) << endl;

//     return 0;
// }