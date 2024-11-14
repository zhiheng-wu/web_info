#include <iostream>
#include <vector>
#include <string>
#include<Windows.h>
#pragma execution_character_set("utf-8")
using namespace std;

// 获取标签列表的函数，直到输入空行表示结束
void getLabels2(const std::string& prompt, std::vector<std::string>& labels) {
    std::string label;
    std::cout << prompt << "\n请输入标签（按回车不输入任何内容结束输入）：" << std::endl;
    while (true) {
        std::getline(std::cin, label);  // 获取一行输入
        if (label.empty()) {
            break;  // 空行表示结束输入
        }
        labels.push_back(label);  // 非空行添加到标签列表
    }
}

int main4() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::vector<std::string> andLabels, orLabels, notLabels;

    // 获取AND标签
    getLabels2("AND:", andLabels);

    // 获取OR标签
    getLabels2("OR:", orLabels);

    // 获取NOT标签
    getLabels2("NOT:", notLabels);

    // 输出结果
    std::cout << "\nAND 标签：";
    for (const auto& label : andLabels) {
        std::cout << label << " ";
    }
    std::cout << std::endl;

    std::cout << "OR 标签：";
    for (const auto& label : orLabels) {
        std::cout << label << " ";
    }
    std::cout << std::endl;

    std::cout << "NOT 标签：";
    for (const auto& label : notLabels) {
        std::cout << label << " ";
    }
    std::cout << std::endl;

    

    return 0;
}
