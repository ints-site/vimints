#include <ncurses.h>
#include "../include/editor.h"
#include "../include/ui_ncurses.h"
#include <string>
#include <vector>

NCursesUI::NCursesUI(Editor& editor) : m_editor(editor), m_statusMessage("") {
    initScreen();
}

void NCursesUI::initScreen() {
    // 初始化 ncurses
    initscr();            
    cbreak();             
    noecho();             
    keypad(stdscr, TRUE); 
    
    // 启用颜色
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);   // 普通模式
    init_pair(2, COLOR_GREEN, COLOR_BLACK);   // 插入模式
    init_pair(3, COLOR_RED, COLOR_BLACK);     // 命令模式
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);  // 可视模式

    // 创建窗口
    int height = LINES - 1;
    int width = COLS;
    m_mainWin = newwin(height, width, 0, 0);
    m_statusWin = newwin(1, width, height, 0);
}

void NCursesUI::renderContent() {
    wclear(m_mainWin);
    
    // 渲染文件内容
    const auto& lines = m_editor.getLines();
    for (size_t i = 0; i < lines.size(); ++i) {
        mvwprintw(m_mainWin, i, 0, "%s", lines[i].c_str());
    }
    
    // 高亮当前行
    int currentLine = m_editor.getCursorLine();
    mvwchgat(m_mainWin, currentLine, 0, -1, A_REVERSE, 0, NULL);
    
    wrefresh(m_mainWin);
}

void NCursesUI::renderStatusBar() {
    wclear(m_statusWin);
    
    std::string modeStr = getModeString();
    std::string statusLine = "Mode: " + modeStr + " | File: " + 
        (m_editor.getCurrentFile().empty() ? "Untitled" : m_editor.getCurrentFile()) +
        " | " + m_statusMessage;
    
    // 根据模式设置颜色
    int colorPair = 1;
    switch(m_editor.getMode()) {
        case EditorMode::INSERT: colorPair = 2; break;
        case EditorMode::COMMAND: colorPair = 3; break;
        case EditorMode::VISUAL_CHAR:
        case EditorMode::VISUAL_LINE:
        case EditorMode::VISUAL_BLOCK:
            colorPair = 4; break;
    }
    
    wattron(m_statusWin, COLOR_PAIR(colorPair));
    mvwprintw(m_statusWin, 0, 0, "%s", statusLine.c_str());
    wattroff(m_statusWin, COLOR_PAIR(colorPair));
    
    wrefresh(m_statusWin);
}

std::string NCursesUI::getModeString() {
    switch(m_editor.getMode()) {
        case EditorMode::NORMAL: return "NORMAL";
        case EditorMode::INSERT: return "INSERT";
        case EditorMode::COMMAND: return "COMMAND";
        case EditorMode::VISUAL_CHAR: return "VISUAL CHAR";
        case EditorMode::VISUAL_LINE: return "VISUAL LINE";
        case EditorMode::VISUAL_BLOCK: return "VISUAL BLOCK";
        default: return "UNKNOWN";
    }
}

void NCursesUI::run() {
    while (true) {
        renderContent();
        renderStatusBar();
        
        int ch = getch();
        processKeyInput(ch);
    }
}

void NCursesUI::processKeyInput(int ch) {
    switch(m_editor.getMode()) {
        case EditorMode::NORMAL:
            handleNormalMode(ch);
            break;
        case EditorMode::INSERT:
            handleInsertMode(ch);
            break;
        case EditorMode::COMMAND:
            handleCommandMode(ch);
            break;
        case EditorMode::VISUAL_CHAR:
        case EditorMode::VISUAL_LINE:
        case EditorMode::VISUAL_BLOCK:
            // TODO: 实现可视模式处理
            break;
    }
}

void NCursesUI::handleNormalMode(int ch) {
    switch(ch) {
        case 'i': 
            m_editor.setMode(EditorMode::INSERT);
            break;
        case 'h': m_editor.moveCursorLeft(); break;
        case 'j': m_editor.moveCursorDown(); break;
        case 'k': m_editor.moveCursorUp(); break;
        case 'l': m_editor.moveCursorRight(); break;
        case 'x': m_editor.deleteText(DeleteType::CHARACTER); break;
        case 'v': m_editor.setMode(EditorMode::VISUAL_CHAR); break;
        case ':': 
            m_editor.setMode(EditorMode::COMMAND);
            m_statusMessage = ":";
            break;
    }
}

void NCursesUI::handleInsertMode(int ch) {
    switch(ch) {
        case 27: // ESC
            m_editor.setMode(EditorMode::NORMAL);
            break;
        case KEY_BACKSPACE:
        case 127:
            // 处理退格
            break;
        default:
            if (ch >= 32 && ch < 127) {
                m_editor.insertText(std::string(1, static_cast<char>(ch)));
            }
            break;
    }
}

void NCursesUI::handleCommandMode(int ch) {
    switch(ch) {
        case 27: // ESC
            m_editor.setMode(EditorMode::NORMAL);
            m_statusMessage = "";
            break;
        case 10: // 回车
            processCommand();
            break;
        case KEY_BACKSPACE:
        case 127:
            if (m_statusMessage.length() > 1) {
                m_statusMessage = m_statusMessage.substr(0, m_statusMessage.length() - 1);
            }
            break;
        default:
            if (ch >= 32 && ch < 127) {
                m_statusMessage += static_cast<char>(ch);
            }
            break;
    }
}

void NCursesUI::processCommand() {
    if (m_statusMessage.length() > 1 && m_statusMessage[0] == ':') {
        std::string command = m_statusMessage.substr(1);
        if (m_editor.executeCommand(command)) {
            m_statusMessage = "命令执行成功";
            if (command == "q" || command.find("wq") != std::string::npos) {
                endwin();
                exit(0);
            }
        } else {
            m_statusMessage = "无效命令";
        }
    }
    m_editor.setMode(EditorMode::NORMAL);
}

NCursesUI::~NCursesUI() {
    endwin();
} 