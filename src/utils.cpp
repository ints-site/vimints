/**
 * @file utils.cpp
 * @brief 实用工具函数实现
 */
#include "../include/utils.h"
#include <iostream>
void printUTF8(const std::string& text) {
    #ifdef _WIN32
    int wideCharLength = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
    std::vector<wchar_t> wideCharBuffer(wideCharLength);
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, wideCharBuffer.data(), wideCharLength);
    
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wideCharBuffer.data(), wideCharLength - 1, NULL, NULL);
    #else
    std::cout << text;
    #endif
}
