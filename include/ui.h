/**
 * @file ui.h
 * @brief 用户界面处理
 * 
 * 大纲：
 * 1. 定义UI类
 * 2. 终端渲染方法
 * 3. 用户输入处理
 * 4. 状态栏和消息显示
 */
#ifndef UI_H
#define UI_H
#include <string>
#include "../include/editor.h"
class UI {
public:
    UI(Editor& editor);
    void render();
    void handleInput();
    void displayStatusBar();
    void displayMessage(const std::string& message);
private:
    Editor& m_editor;
    std::string m_statusMessage;
};
#endif // UI_H
