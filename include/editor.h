/**
 * @file editor.h
 * @brief 编辑器核心类定义
 * 
 * 大纲：
 * 1. 定义编辑器状态枚举
 * 2. 编辑器类声明
 *    - 文件操作方法
 *    - 编辑操作方法
 *    - 模式管理
 *    - 光标移动
 *    - 文本操作
 * 3. 私有成员变量
 *    - 文本缓冲区
 *    - 当前文件路径
 *    - 编辑状态
 */
#ifndef EDITOR_H
#define EDITOR_H
#include <string>
#include <vector>
#include <utility>

// 新增 DeleteType 枚举
enum class DeleteType {
    CHARACTER,
    WORD,
    LINE
};

enum class EditorMode {
    NORMAL,
    INSERT,
    VISUAL_CHAR,
    VISUAL_LINE,
    VISUAL_BLOCK,
    COMMAND
};

class Editor {
private:
    std::vector<std::string> m_lines;
    std::string m_currentFile;
    int m_cursorLine;
    int m_cursorColumn;
    EditorMode m_currentMode;
    EditorMode m_visualMode;
    int m_visualStartLine;
    int m_visualStartColumn;
    std::string m_copiedText;

public:
    Editor();
    ~Editor();

    // 文件操作
    bool openFile(const std::string& filename);
    bool saveFile();
    bool saveFileAs(const std::string& filename);

    // 编辑操作
    void insertText(const std::string& text);
    void deleteText();
    void copyText();
    void pasteText();

    // 光标移动
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorLeft();
    void moveCursorRight();
    void moveToLineStart();
    void moveToLineEnd();
    void moveWordForward();
    void moveWordBackward();

    // 模式管理
    void setMode(EditorMode mode);
    EditorMode getMode() const;

    // 新增方法
    const std::vector<std::string>& getLines() const;
    const std::string& getCurrentFile() const;
    std::string getCurrentLineText() const;
    std::pair<int, int> getCursorPosition() const;

    void startVisualMode(EditorMode visualMode);
    void selectText();
    bool searchText(const std::string& pattern);
    int replaceText(const std::string& oldText, const std::string& newText, bool global = false);

    void clearLines();
    void addEmptyLine();
    void resetCurrentFile();

    // 新增方法用于修改特定行
    void updateLine(int lineIndex, const std::string& newContent);
    void removeLine(int lineIndex);
    void insertLine(int lineIndex, const std::string& line);

    // 添加光标列的 getter 和 setter
    int getCursorColumn() const;
    void setCursorColumn(int column);

    // 命令模式相关方法
    bool executeCommand(const std::string& command);
    std::vector<std::string> splitCommand(const std::string& command);

    // 添加 deleteText 的重载声明
    void deleteText(DeleteType type);
};

// 新增 VimEditor 类，继承自 Editor
class VimEditor : public Editor {
private:
    // 新增私有方法声明
    void handleNormalModeInput(char key);
    void handleInsertModeInput(char key);
    void handleVisualModeInput(char key);
    void handleCommandModeInput(char key);

public:
    void processKeyInput(char key);
    void switchMode(EditorMode newMode);
    void moveCursor(char direction);
    void deleteText(DeleteType type);
};

#endif // EDITOR_H
