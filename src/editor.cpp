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
    if (static_cast<size_t>(m_cursorLine) >= m_lines.size()) {
        m_lines.push_back(text);
    } else {
        m_lines.insert(m_lines.begin() + m_cursorLine, text);
    }
}
// 删除文本
void Editor::deleteText() {
    if (!m_lines.empty() && static_cast<size_t>(m_cursorLine) < m_lines.size()) {
        m_lines.erase(m_lines.begin() + m_cursorLine);
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
    }
}
void Editor::moveCursorDown() {
    if (static_cast<size_t>(m_cursorLine) < m_lines.size() - 1) {
        m_cursorLine++;
    }
}
void Editor::moveCursorLeft() {
    if (m_cursorColumn > 0) {
        m_cursorColumn--;
    }
}
void Editor::moveCursorRight() {
    if (static_cast<size_t>(m_cursorColumn) < m_lines[m_cursorLine].length()) {
        m_cursorColumn++;
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
