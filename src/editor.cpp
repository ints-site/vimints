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
#include <sstream>
// 构造函数
Editor::Editor() : 
    m_cursorLine(0),
    m_cursorColumn(0),
    m_currentMode(EditorMode::NORMAL),
    m_visualMode(EditorMode::NORMAL),
    m_visualStartLine(0),
    m_visualStartColumn(0),
    m_copiedText("") {}
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
// 复制文本
void Editor::copyText() {
    if (m_cursorLine >= 0 && m_cursorLine < static_cast<int>(m_lines.size())) {
        m_copiedText = m_lines[static_cast<std::vector<std::string>::size_type>(m_cursorLine)];
    }
}
// 粘贴文本
void Editor::pasteText() {
    if (!m_copiedText.empty()) {
        // 在当前行插入复制的文本
        insertText(m_copiedText);
    }
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

void Editor::moveToLineStart() {
    m_cursorColumn = 0;
}

void Editor::moveToLineEnd() {
    m_cursorColumn = static_cast<int>(m_lines[m_cursorLine].length());
}

void Editor::moveWordForward() {
    std::string currentLine = m_lines[m_cursorLine];
    while (m_cursorColumn < static_cast<int>(currentLine.length()) && 
           std::isalnum(currentLine[m_cursorColumn])) {
        m_cursorColumn++;
    }
    // Skip whitespace
    while (m_cursorColumn < static_cast<int>(currentLine.length()) && 
           std::isspace(currentLine[m_cursorColumn])) {
        m_cursorColumn++;
    }
}

void Editor::moveWordBackward() {
    std::string currentLine = m_lines[m_cursorLine];
    // Skip leading whitespace
    while (m_cursorColumn > 0 && std::isspace(currentLine[m_cursorColumn - 1])) {
        m_cursorColumn--;
    }
    // Move back to word start
    while (m_cursorColumn > 0 && std::isalnum(currentLine[m_cursorColumn - 1])) {
        m_cursorColumn--;
    }
}

void Editor::startVisualMode(EditorMode visualMode) {
    m_currentMode = visualMode;
    m_visualStartLine = m_cursorLine;
    m_visualStartColumn = m_cursorColumn;
}

void Editor::selectText() {
    // Implement text selection logic based on visual mode
    // This is a simplified version and can be expanded
    switch (m_currentMode) {
        case EditorMode::VISUAL_CHAR:
            // Character-level selection
            break;
        case EditorMode::VISUAL_LINE:
            // Line-level selection
            break;
        case EditorMode::VISUAL_BLOCK:
            // Block-level selection
            break;
        default:
            break;
    }
}

bool Editor::searchText(const std::string& pattern) {
    for (size_t i = 0; i < m_lines.size(); ++i) {
        size_t pos = m_lines[i].find(pattern);
        if (pos != std::string::npos) {
            m_cursorLine = static_cast<int>(i);
            m_cursorColumn = static_cast<int>(pos);
            return true;
        }
    }
    return false;
}

int Editor::replaceText(const std::string& oldText, const std::string& newText, bool global) {
    int replacementCount = 0;
    for (auto& line : m_lines) {
        size_t pos = 0;
        while ((pos = line.find(oldText, pos)) != std::string::npos) {
            line.replace(pos, oldText.length(), newText);
            pos += newText.length();
            replacementCount++;
            
            if (!global) break;
        }
    }
    return replacementCount;
}

// 实现新增的公共方法
void Editor::clearLines() {
    m_lines.clear();
}

void Editor::addEmptyLine() {
    m_lines.push_back("");
}

void Editor::resetCurrentFile() {
    m_currentFile = "";
}

void VimEditor::processKeyInput(char key) {
    switch(getMode()) {
        case EditorMode::NORMAL:
            handleNormalModeInput(key);
            break;
        case EditorMode::INSERT:
            handleInsertModeInput(key);
            break;
        case EditorMode::VISUAL_CHAR:
        case EditorMode::VISUAL_LINE:
        case EditorMode::VISUAL_BLOCK:
            handleVisualModeInput(key);
            break;
        case EditorMode::COMMAND:
            handleCommandModeInput(key);
            break;
    }
}

void VimEditor::handleNormalModeInput(char key) {
    switch(key) {
        case 'i': 
            switchMode(EditorMode::INSERT); 
            break;
        case 'v': 
            switchMode(EditorMode::VISUAL_CHAR); 
            break;
        case 'V': 
            switchMode(EditorMode::VISUAL_LINE); 
            break;
        case 'h': moveCursorLeft(); break;
        case 'j': moveCursorDown(); break;
        case 'k': moveCursorUp(); break;
        case 'l': moveCursorRight(); break;
        case '0': moveToLineStart(); break;
        case '$': moveToLineEnd(); break;
        case 'w': moveWordForward(); break;
        case 'b': moveWordBackward(); break;
        // 添加更多普通模式命令
    }
}

void VimEditor::handleInsertModeInput(char key) {
    // 在插入模式下，大多数字符都会被插入
    if (key == 27) { // ESC键
        switchMode(EditorMode::NORMAL);
    } else {
        std::string keyStr(1, key);
        insertText(keyStr);
    }
}

void VimEditor::handleVisualModeInput(char key) {
    switch(key) {
        case 27: // ESC键
            switchMode(EditorMode::NORMAL);
            break;
        case 'h': moveCursorLeft(); break;
        case 'j': moveCursorDown(); break;
        case 'k': moveCursorUp(); break;
        case 'l': moveCursorRight(); break;
        // 添加文本选择逻辑
        case 'y': 
            copyText(); 
            switchMode(EditorMode::NORMAL);
            break;
        case 'd': 
            // 根据当前可视模式选择删除类型
            switch(getMode()) {
                case EditorMode::VISUAL_CHAR:
                    deleteText(DeleteType::CHARACTER);
                    break;
                case EditorMode::VISUAL_LINE:
                    deleteText(DeleteType::LINE);
                    break;
                case EditorMode::VISUAL_BLOCK:
                    // 可以添加块删除的特殊逻辑
                    deleteText(DeleteType::CHARACTER);
                    break;
                default:
                    break;
            }
            switchMode(EditorMode::NORMAL);
            break;
    }
}

void VimEditor::handleCommandModeInput(char key) {
    // 这里可以添加命令模式下的输入处理逻辑
    // 例如处理命令输入、退出命令模式等
    switch(key) {
        case 27: // ESC键
            switchMode(EditorMode::NORMAL);
            break;
        // 可以添加更多命令模式下的按键处理
    }
}

void VimEditor::switchMode(EditorMode newMode) {
    setMode(newMode);
}

void VimEditor::moveCursor(char direction) {
    switch(direction) {
        case 'h': moveCursorLeft(); break;
        case 'j': moveCursorDown(); break;
        case 'k': moveCursorUp(); break;
        case 'l': moveCursorRight(); break;
    }
}
void VimEditor::deleteText(DeleteType type) {
    switch(type) {
        case DeleteType::CHARACTER:
            Editor::deleteText();  // 调用基类的删除方法
            break;
        case DeleteType::WORD: {
            // 删除当前单词
            std::string currentLine = getCurrentLineText();
            size_t wordEnd = currentLine.find_first_of(" \t", getCursorPosition().second);
            if (wordEnd != std::string::npos) {
                currentLine.erase(getCursorPosition().second, wordEnd - getCursorPosition().second);
            } else {
                currentLine.erase(getCursorPosition().second);
            }
            // 更新当前行的文本
            updateLine(getCursorPosition().first, currentLine);
            break;
        }
        case DeleteType::LINE:
            // 删除整行
            removeLine(getCursorPosition().first);
            break;
    }
}

// 实现新增的方法
void Editor::updateLine(int lineIndex, const std::string& newContent) {
    if (lineIndex >= 0 && lineIndex < static_cast<int>(m_lines.size())) {
        m_lines[static_cast<std::vector<std::string>::size_type>(lineIndex)] = newContent;
    }
}

void Editor::removeLine(int lineIndex) {
    if (lineIndex >= 0 && lineIndex < static_cast<int>(m_lines.size())) {
        m_lines.erase(m_lines.begin() + static_cast<std::vector<std::string>::size_type>(lineIndex));
        
        // 如果删除后没有行，添加一个空行
        if (m_lines.empty()) {
            m_lines.push_back("");
        }
    }
}

void Editor::insertLine(int lineIndex, const std::string& line) {
    if (lineIndex >= 0 && lineIndex <= static_cast<int>(m_lines.size())) {
        m_lines.insert(m_lines.begin() + static_cast<std::vector<std::string>::size_type>(lineIndex), line);
    }
}

int Editor::getCursorColumn() const {
    return m_cursorColumn;
}

void Editor::setCursorColumn(int column) {
    // 确保列在有效范围内
    if (!m_lines.empty()) {
        int maxColumn = m_lines[m_cursorLine].length();
        m_cursorColumn = std::max(0, std::min(column, maxColumn));
    } else {
        m_cursorColumn = 0;
    }
}

bool Editor::executeCommand(const std::string& command) {
    std::vector<std::string> parts = splitCommand(command);
    
    if (parts.empty()) return false;

    if (parts[0] == "w" || parts[0] == "write") {
        // 保存文件
        if (parts.size() > 1) {
            return saveFileAs(parts[1]);
        } else {
            return saveFile();
        }
    } else if (parts[0] == "q" || parts[0] == "quit") {
        // 退出编辑器的逻辑应该在 UI 层处理
        return true;
    } else if (parts[0] == "wq") {
        // 保存并退出
        if (parts.size() > 1) {
            saveFileAs(parts[1]);
        } else {
            saveFile();
        }
        return true;
    } else if (parts[0] == "e" || parts[0] == "edit") {
        // 打开文件
        if (parts.size() > 1) {
            return openFile(parts[1]);
        }
    } else if (parts[0] == "set") {
        // 设置编辑器选项
        // 可以在这里添加一些配置选项
    }

    return false;
}

std::vector<std::string> Editor::splitCommand(const std::string& command) {
    std::vector<std::string> parts;
    std::istringstream iss(command);
    std::string part;
    
    while (iss >> part) {
        parts.push_back(part);
    }
    
    return parts;
}
