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
    if (cmd == "quit" || cmd == "q") {
        exit(0);  // 直接退出
    }
    
    if (cmd == "save" || cmd == "w") {
        if (!args.empty()) {
            return m_editor.saveFileAs(args[0]);
        }
        return m_editor.saveFile();
    }
    
    if (cmd == "open" || cmd == "e") {
        if (!args.empty()) {
            return m_editor.openFile(args[0]);
        }
        return false;
    }
    
    if (cmd == "new" || cmd == "n") {
        // 使用新增的公共方法
        m_editor.clearLines();
        m_editor.addEmptyLine();
        m_editor.resetCurrentFile();
        return true;
    }
    
    return false;
}
