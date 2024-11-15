/**
 * @file main.cpp
 * @brief 跨平台命令行文本编辑器 - 程序入口
 * 
 * 大纲：
 * 1. 包含必要的头文件
 * 2. 解析命令行参数
 * 3. 初始化编辑器
 * 4. 主程序循环
 * 5. 异常处理和资源清理
 */
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include "../include/editor.h"
#include "../include/ui.h"
#include "../include/command.h"
#include "../include/utils.h"
int main(int argc, char* argv[]) {
    // Windows 平台特定的编码设置
    #ifdef _WIN32
    // 设置控制台输出代码页为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 设置控制台输入代码页为 UTF-8
    SetConsoleCP(CP_UTF8);
    // 设置标准输出为 Unicode 模式
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    
    // 尝试设置本地化，使用更通用的方法
    try {
        std::locale::global(std::locale(""));  // 使用系统默认区域设置
    } catch (const std::runtime_error&) {
        // 如果失败，使用 C 区域设置
        std::locale::global(std::locale::classic());
    }
    
    try {
        // 创建编辑器实例
        Editor editor;
        
        // 创建UI实例
        UI ui(editor);
        
        // 创建命令处理器实例
        CommandProcessor cmdProcessor(editor);
        
        // 处理命令行参数
        if (argc > 1) {
            std::string filename(argv[1]);
            if (!editor.openFile(filename)) {
                printUTF8(std::string("无法打开文件: ") + filename + "\n");
                return 1;
            }
        }
        
        // 主程序循环
        bool running = true;
        while (running) {
            // 渲染界面
            ui.render();
            
            // 处理用户输入
            ui.handleInput();
            
            // TODO: 添加退出条件
        }
    }
    catch (const std::exception& e) {
        printUTF8(std::string("发生错误: ") + e.what() + "\n");
        return 1;
    }
    
    return 0;
}
