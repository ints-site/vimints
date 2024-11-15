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
enum class EditorMode {
    NORMAL,
    INSERT,
    COMMAND
};
class Editor {
public:
    // 构造函数和析构函数
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
    // 模式管理
    void setMode(EditorMode mode);
    EditorMode getMode() const;
    // 新增的公共访问方法
    const std::vector<std::string>& getLines() const;
    const std::string& getCurrentFile() const;
    // 公开光标位置，允许 UI 直接管理
    int m_cursorLine;
    int m_cursorColumn;
    // 新增方法：获取当前行文本
    std::string getCurrentLineText() const;
    // 新增方法：获取当前光标位置
    std::pair<int, int> getCursorPosition() const;
private:
    // 文本缓冲区
    std::vector<std::string> m_lines;
    // 当前文件路径
    std::string m_currentFile;
    // 当前编辑模式
    EditorMode m_currentMode;
};
#endif // EDITOR_H
