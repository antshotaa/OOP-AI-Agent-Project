#include "calc_tool.h"
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <stack>

#define MAX_SIZE 1000
using namespace std;

string Calculator::name(){
    return "Calculator";
}

string Calculator::description(){
    return "Calculate mathematical expression.";
}


nlohmann::json Calculator::exec(nlohmann::json& args){ // thuc hien tinh toan
    string expression = args.at("expression").get<string>();
    if (expression.empty()){
        return {{"error","Biểu thức rỗng."}};
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
        if (expression[i] < '0' || 
            expression[i] > '9') 
            return {{"error", "Biểu thức không hợp lệ."}};
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
    for (int i = 0; i < p1; i++) {
        cout << "nums[" << i << "] = " << nums[i] << endl;
    }
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



int main() {
    Calculator calc;

    nlohmann::json args = {
        {"expression", " "}
    };

    nlohmann::json result = calc.exec(args);

    cout << result.dump(4) << endl;

    return 0;
}