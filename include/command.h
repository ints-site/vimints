/**
 * @file command.h
 * @brief 命令处理和解析
 * 
 * 大纲：
 * 1. 定义命令处理类
 * 2. 命令解析方法
 * 3. 支持的命令类型
 * 4. 命令执行接口
 */
#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <vector>
#include "../include/editor.h"
class CommandProcessor {
public:
    CommandProcessor(Editor& editor);
    bool processCommand(const std::string& command);
private:
    Editor& m_editor;
    std::vector<std::string> splitCommand(const std::string& command);
    bool executeCommand(const std::string& cmd, const std::vector<std::string>& args);
};
#endif // COMMAND_H
