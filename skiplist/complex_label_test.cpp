#include <iostream>
#include <vector>
#include <string>
#include<Windows.h>
#pragma execution_character_set("utf-8")
using namespace std;

// ��ȡ��ǩ�б�ĺ�����ֱ��������б�ʾ����
void getLabels2(const std::string& prompt, std::vector<std::string>& labels) {
    std::string label;
    std::cout << prompt << "\n�������ǩ�����س��������κ����ݽ������룩��" << std::endl;
    while (true) {
        std::getline(std::cin, label);  // ��ȡһ������
        if (label.empty()) {
            break;  // ���б�ʾ��������
        }
        labels.push_back(label);  // �ǿ�����ӵ���ǩ�б�
    }
}

int main4() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::vector<std::string> andLabels, orLabels, notLabels;

    // ��ȡAND��ǩ
    getLabels2("AND:", andLabels);

    // ��ȡOR��ǩ
    getLabels2("OR:", orLabels);

    // ��ȡNOT��ǩ
    getLabels2("NOT:", notLabels);

    // ������
    std::cout << "\nAND ��ǩ��";
    for (const auto& label : andLabels) {
        std::cout << label << " ";
    }
    std::cout << std::endl;

    std::cout << "OR ��ǩ��";
    for (const auto& label : orLabels) {
        std::cout << label << " ";
    }
    std::cout << std::endl;

    std::cout << "NOT ��ǩ��";
    for (const auto& label : notLabels) {
        std::cout << label << " ";
    }
    std::cout << std::endl;

    

    return 0;
}
