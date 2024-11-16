/**
 * @file ui.cpp
 * @brief 用户界面处理实现
 * 
 * 大纲：
 * 1. 渲染编辑器界面
 * 2. 处理用户输入
 * 3. 状态栏和消息显示
 */
#include "../include/ui.h"
#include "../include/utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
// 跨平台的非阻塞输入实现
int getChar() {
    #ifdef _WIN32
    return _getch();
    #else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
    #endif
}
UI::UI(Editor& editor) : m_editor(editor), m_statusMessage("") {}
void UI::render() {
    // 使用更高效的渲染方式，避免全屏清除导致闪烁
    printUTF8("\x1b[2J");    // 清除整个屏幕
    printUTF8("\x1b[H");     // 将光标移动到左上角

    // 渲染文件内容
    const auto& lines = m_editor.getLines();
    for (size_t i = 0; i < lines.size(); ++i) {
        printUTF8(std::to_string(i + 1) + " | " + lines[i] + "\n");
    }
    
    displayStatusBar();
    
    // 显示光标
    printUTF8("\x1b[?25h");
}
void UI::handleInput() {
    int ch = getChar();
    switch (m_editor.getMode()) {
        case EditorMode::NORMAL:
        case EditorMode::VISUAL_CHAR:
        case EditorMode::VISUAL_LINE:
        case EditorMode::VISUAL_BLOCK:
            // 统一处理普通和可视模式
            switch (ch) {
                case 'i': 
                    m_editor.setMode(EditorMode::INSERT); 
                    m_statusMessage = "-- 插入模式 --";
                    break;
                case 'k': m_editor.moveCursorUp(); break;
                case 'j': m_editor.moveCursorDown(); break;
                case 'h': m_editor.moveCursorLeft(); break;
                case 'l': m_editor.moveCursorRight(); break;
                case 'x': m_editor.deleteText(); break;
                case ':': 
                    m_editor.setMode(EditorMode::COMMAND);
                    m_statusMessage = ":";
                    break;
                case 'o': 
                    m_editor.setMode(EditorMode::INSERT);
                    m_editor.insertText("");  // 在当前行下方插入新行
                    m_statusMessage = "-- 插入模式 --";
                    break;
                case 'O': 
                    m_editor.setMode(EditorMode::INSERT);
                    m_editor.setCursorColumn(0);
                    m_editor.insertText("");  // 在当前行上方插入新行
                    m_statusMessage = "-- 插入模式 --";
                    break;
                case 'y': 
                    m_editor.copyText(); 
                    m_statusMessage = "已复制行";
                    break;
                case 'p': 
                    m_editor.pasteText(); 
                    m_statusMessage = "已粘贴";
                    break;
                case '0': m_editor.moveToLineStart(); break;
                case '$': m_editor.moveToLineEnd(); break;
                case 'w': m_editor.moveWordForward(); break;
                case 'b': m_editor.moveWordBackward(); break;
                
                case 'v': 
                    m_editor.startVisualMode(EditorMode::VISUAL_CHAR);
                    m_statusMessage = "-- VISUAL --";
                    break;
                case 'V': 
                    m_editor.startVisualMode(EditorMode::VISUAL_LINE);
                    m_statusMessage = "-- VISUAL LINE --";
                    break;
                
                case '/':
                    m_editor.setMode(EditorMode::COMMAND);
                    m_statusMessage = "搜索: ";
                    break;
                case 27: // ESC键
                    m_editor.setMode(EditorMode::NORMAL);
                    m_statusMessage = "";
                    break;
            }
            break;
        
        case EditorMode::INSERT:
            switch (ch) {
                case 27: // ESC键
                    m_editor.setMode(EditorMode::NORMAL);
                    m_statusMessage = "";
                    break;
                case 13: // 回车键
                    m_editor.insertText("");  // 插入空行
                    break;
                case 127: // 退格键
                    if (m_editor.getCursorColumn() > 0) {
                        m_editor.setCursorColumn(m_editor.getCursorColumn() - 1);
                        m_editor.deleteText();
                    }
                    break;
                default:
                    // 直接在当前行插入字符
                    if (ch >= 32 && ch < 127) {  // 可打印字符
                        std::string charStr(1, static_cast<char>(ch));
                        m_editor.insertText(charStr);
                    }
                    break;
            }
            break;
        
        case EditorMode::COMMAND:
            switch (ch) {
                case 27: // ESC键
                    m_editor.setMode(EditorMode::NORMAL);
                    m_statusMessage = "";
                    break;
                case 13: // 回车键
                    if (m_statusMessage.length() > 1 && m_statusMessage[0] == ':') {
                        std::string command = m_statusMessage.substr(1);
                        if (m_editor.executeCommand(command)) {
                            if (command == "q" || command.find("wq") != std::string::npos) {
                                exit(0);  // 退出程序
                            }
                            m_statusMessage = "命令执行成功";
                        } else {
                            m_statusMessage = "无效命令";
                        }
                    }
                    break;
                case 127: // 退格键
                    if (m_statusMessage.length() > 1) {
                        m_statusMessage = m_statusMessage.substr(0, m_statusMessage.length() - 1);
                    }
                    break;
                default:
                    // 在命令模式下输入字符
                    if (ch >= 32 && ch < 127) {
                        m_statusMessage += static_cast<char>(ch);
                    }
                    break;
            }
            break;
    }
}
void UI::displayStatusBar() {
    // 显示当前模式和状态信息
    std::string modeStr;
    switch (m_editor.getMode()) {
        case EditorMode::NORMAL: modeStr = "普通模式"; break;
        case EditorMode::INSERT: modeStr = "插入模式"; break;
        case EditorMode::COMMAND: modeStr = "命令模式"; break;
        case EditorMode::VISUAL_CHAR: modeStr = "可视字符模式"; break;
        case EditorMode::VISUAL_LINE: modeStr = "可视行模式"; break;
        case EditorMode::VISUAL_BLOCK: modeStr = "可视块模式"; break;
    }
    
    std::string statusLine = "模式: " + modeStr + " | 文件: " + 
        (m_editor.getCurrentFile().empty() ? "未命名" : m_editor.getCurrentFile()) + 
        " | " + m_statusMessage;
    
    printUTF8(statusLine + "\n");
}
void UI::displayMessage(const std::string& message) {
    m_statusMessage = message;
}
