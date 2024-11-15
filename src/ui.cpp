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
// Windows下的UTF-8到UTF-16转换输出函数
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
UI::UI(Editor& editor) : m_editor(editor), m_statusMessage("") {}
void UI::render() {
    // 清屏（跨平台）
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    
    // 渲染文件内容
    const auto& lines = m_editor.getLines();
    for (size_t i = 0; i < lines.size(); ++i) {
        printUTF8(std::to_string(i + 1) + " | " + lines[i] + "\n");
    }
    
    displayStatusBar();
}
void UI::handleInput() {
    int ch = getChar();  // 使用跨平台的输入函数
    switch (m_editor.getMode()) {
        case EditorMode::NORMAL:
            switch (ch) {
                case 'i': m_editor.setMode(EditorMode::INSERT); break;
                case 'k': m_editor.moveCursorUp(); break;
                case 'j': m_editor.moveCursorDown(); break;
                case 'h': m_editor.moveCursorLeft(); break;
                case 'l': m_editor.moveCursorRight(); break;
                case 'x': m_editor.deleteText(); break;
                case ':': m_editor.setMode(EditorMode::COMMAND); break;
            }
            break;
        
        case EditorMode::INSERT:
            switch (ch) {
                case 27: // ESC键
                    m_editor.setMode(EditorMode::NORMAL);
                    break;
                default:
                    // 插入字符
                    m_editor.insertText(std::string(1, ch));
                    break;
            }
            break;
        
        case EditorMode::COMMAND:
            switch (ch) {
                case 27: // ESC键
                    m_editor.setMode(EditorMode::NORMAL);
                    break;
                case 'w': 
                    if (m_editor.saveFile()) {
                        m_statusMessage = "文件已保存";
                    } else {
                        m_statusMessage = "保存失败";
                    }
                    break;
                case 'q': 
                    // TODO: 实现退出逻辑
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
    }
    
    std::string statusLine = "模式: " + modeStr + " | 文件: " + 
        (m_editor.getCurrentFile().empty() ? "未命名" : m_editor.getCurrentFile()) + 
        " | " + m_statusMessage;
    
    printUTF8(statusLine + "\n");
}
void UI::displayMessage(const std::string& message) {
    m_statusMessage = message;
}
