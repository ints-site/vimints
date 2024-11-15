/**
 * @file editor.cpp
 * @brief 编辑器核心类实现
 * 
 * 大纲：
 * 1. 构造函数和析构函数实现
 * 2. 文件操作方法实现
 * 3. 编辑操作方法实现
 * 4. 光标移动方法实现
 * 5. 模式管理方法实现
 */
#include "../include/editor.h"
#include <fstream>
#include <iostream>
// 构造函数
Editor::Editor() : 
    m_currentMode(EditorMode::NORMAL),
    m_cursorLine(0),
    m_cursorColumn(0) {}
// 析构函数
Editor::~Editor() {}
// 打开文件
bool Editor::openFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }
    
    m_lines.clear();
    std::string line;
    while (std::getline(file, line)) {
        m_lines.push_back(line);
    }
    
    m_currentFile = filename;
    return true;
}
// 保存文件
bool Editor::saveFile() {
    if (m_currentFile.empty()) {
        std::cerr << "未指定文件名" << std::endl;
        return false;
    }
    return saveFileAs(m_currentFile);
}
// 另存为
bool Editor::saveFileAs(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法保存文件: " << filename << std::endl;
        return false;
    }
    
    for (const auto& line : m_lines) {
        file << line << std::endl;
    }
    
    m_currentFile = filename;
    return true;
}
// 插入文本
void Editor::insertText(const std::string& text) {
    // 如果文本为空，插入一个空行
    if (text.empty()) {
        if (m_lines.empty()) {
            m_lines.push_back("");
        } else {
            m_lines.insert(m_lines.begin() + static_cast<std::vector<std::string>::size_type>(m_cursorLine) + 1, "");
            m_cursorLine++;
        }
        return;
    }
    
    // 如果行列表为空，直接添加新行
    if (m_lines.empty()) {
        m_lines.push_back(text);
        m_cursorColumn = text.length(); // 更新光标列
        return;
    }
    
    // 确保光标行有效
    if (m_cursorLine < 0) {
        m_cursorLine = 0;
    }
    if (m_cursorLine >= static_cast<int>(m_lines.size())) {
        m_lines.push_back(""); // 如果超出范围，添加新行
        m_cursorLine = static_cast<int>(m_lines.size()) - 1;
    }

    // 获取当前行的引用
    std::string& currentLine = m_lines[static_cast<std::vector<std::string>::size_type>(m_cursorLine)];
    
    // 确保光标列有效
    if (m_cursorColumn < 0) {
        m_cursorColumn = 0;
    }
    if (m_cursorColumn > static_cast<int>(currentLine.length())) {
        m_cursorColumn = static_cast<int>(currentLine.length());
    }

    // 在当前行的光标位置插入文本
    currentLine = currentLine.substr(0, m_cursorColumn) + text + currentLine.substr(m_cursorColumn);
    
    // 更新光标位置
    m_cursorColumn += text.length();
}
// 删除文本
void Editor::deleteText() {
    if (!m_lines.empty() && m_cursorLine < static_cast<int>(m_lines.size())) {
        std::string& currentLine = m_lines[static_cast<std::vector<std::string>::size_type>(m_cursorLine)];
        
        // 如果光标不在行首，删除光标前的字符
        if (m_cursorColumn > 0 && m_cursorColumn <= static_cast<int>(currentLine.length())) {
            currentLine.erase(m_cursorColumn - 1, 1);
            m_cursorColumn--;
        } 
        // 如果光标在行首且不是第一行，合并当前行和上一行
        else if (m_cursorLine > 0) {
            std::string prevLine = m_lines[static_cast<std::vector<std::string>::size_type>(m_cursorLine - 1)];
            m_cursorColumn = prevLine.length();
            m_lines[static_cast<std::vector<std::string>::size_type>(m_cursorLine - 1)] += currentLine;
            m_lines.erase(m_lines.begin() + static_cast<std::vector<std::string>::size_type>(m_cursorLine));
            m_cursorLine--;
        }

        // 如果删除后没有行，添加一个空行
        if (m_lines.empty()) {
            m_lines.push_back("");
        }
    }
}
// 复制文本（暂时为空实现）
void Editor::copyText() {
    // TODO: 实现复制功能
}
// 粘贴文本（暂时为空实现）
void Editor::pasteText() {
    // TODO: 实现粘贴功能
}
// 光标移动方法
void Editor::moveCursorUp() {
    if (m_cursorLine > 0) {
        m_cursorLine--;
        // 保持光标列在新行的有效范围内
        if (m_cursorColumn > static_cast<int>(m_lines[m_cursorLine].length())) {
            m_cursorColumn = static_cast<int>(m_lines[m_cursorLine].length());
        }
    }
}
void Editor::moveCursorDown() {
    if (m_cursorLine < static_cast<int>(m_lines.size()) - 1) {
        m_cursorLine++;
        // 保持光标列在新行的有效范围内
        if (m_cursorColumn > static_cast<int>(m_lines[m_cursorLine].length())) {
            m_cursorColumn = static_cast<int>(m_lines[m_cursorLine].length());
        }
    }
}
void Editor::moveCursorLeft() {
    if (m_cursorColumn > 0) {
        m_cursorColumn--;
    } else if (m_cursorLine > 0) {
        // 如果在行首，移动到上一行末尾
        m_cursorLine--;
        m_cursorColumn = static_cast<int>(m_lines[m_cursorLine].length());
    }
}
void Editor::moveCursorRight() {
    if (m_cursorColumn < static_cast<int>(m_lines[m_cursorLine].length())) {
        m_cursorColumn++;
    } else if (m_cursorLine < static_cast<int>(m_lines.size()) - 1) {
        // 如果在行尾，移动到下一行行首
        m_cursorLine++;
        m_cursorColumn = 0;
    }
}
// 模式管理
void Editor::setMode(EditorMode mode) {
    m_currentMode = mode;
}
EditorMode Editor::getMode() const {
    return m_currentMode;
}
// 新增的公共访问方法实现
const std::vector<std::string>& Editor::getLines() const {
    return m_lines;
}
const std::string& Editor::getCurrentFile() const {
    return m_currentFile;
}

std::string Editor::getCurrentLineText() const {
    if (m_cursorLine >= 0 && m_cursorLine < static_cast<int>(m_lines.size())) {
        return m_lines[static_cast<std::vector<std::string>::size_type>(m_cursorLine)];
    }
    return "";
}

std::pair<int, int> Editor::getCursorPosition() const {
    return {m_cursorLine, m_cursorColumn};
}
