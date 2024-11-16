#ifndef UI_NCURSES_H
#define UI_NCURSES_H

#include "editor.h"
#include <ncurses.h>
#include <string>

// 原有的类定义保持不变
class NCursesUI {
private:
    Editor& m_editor;
    WINDOW* m_mainWin;
    WINDOW* m_statusWin;
    std::string m_statusMessage;

    void initScreen();
    void renderContent();
    void renderStatusBar();
    std::string getModeString();

    void handleNormalMode(int ch);
    void handleInsertMode(int ch);
    void handleCommandMode(int ch);
    void processCommand();

public:
    NCursesUI(Editor& editor);
    ~NCursesUI();

    void run();
    void processKeyInput(int ch);
};

#endif // UI_NCURSES_H 