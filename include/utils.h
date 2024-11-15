/**
 * @file utils.h
 * @brief 实用工具函数定义
 * 
 * 大纲：
 * 1. 跨平台实用函数
 * 2. 字符串和编码处理
 */
#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
// 跨平台的UTF-8输出函数声明
void printUTF8(const std::string& text);
#endif // UTILS_H
