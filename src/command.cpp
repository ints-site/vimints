#include "../include/command.h"
#include <sstream>
CommandProcessor::CommandProcessor(Editor& editor) : m_editor(editor) {
}
bool CommandProcessor::processCommand(const std::string& command) {
    std::vector<std::string> args = splitCommand(command);
    if (args.empty()) {
        return false;
    }
    
    std::string cmd = args[0];
    args.erase(args.begin()); // 移除命令名称，保留参数
    
    return executeCommand(cmd, args);
}
std::vector<std::string> CommandProcessor::splitCommand(const std::string& command) {
    std::vector<std::string> tokens;
    std::istringstream iss(command);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}
bool CommandProcessor::executeCommand(const std::string& cmd, const std::vector<std::string>& args) {
    // 基本的命令处理逻辑
    if (cmd == "quit" || cmd == "q") {
        // 退出编辑器
        return true;
    }
    
    if (cmd == "save" || cmd == "w") {
        // 保存文件
        if (!args.empty()) {
            // 如果提供了文件名，使用另存为
            return m_editor.saveFileAs(args[0]);
        }
        // 否则保存当前文件
        return m_editor.saveFile();
    }
    
    if (cmd == "open" || cmd == "e") {
        // 打开文件
        if (!args.empty()) {
            return m_editor.openFile(args[0]);
        }
        return false;
    }
    
    // 可以根据需要添加更多命令处理
    
    return false;
}
