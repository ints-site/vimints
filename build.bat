@echo off
REM VimInts 项目构建脚本
REM 设置控制台代码页为 UTF-8
chcp 65001 > nul
REM 检查是否安装了 CMake
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo 错误：未找到 CMake。请确保 CMake 已安装并添加到系统路径。
    exit /b 1
)
REM 创建构建目录
if not exist build mkdir build
cd build
REM 使用 CMake 生成项目文件
REM 可以根据需要更改生成器，例如使用 Visual Studio
cmake -G "MinGW Makefiles" ..
REM 编译项目
cmake --build .
REM 返回项目根目录
cd ..
echo 构建完成。可执行文件位于 build 目录。
