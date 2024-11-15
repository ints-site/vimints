# VimInts 项目 CMake 配置文件
# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)
# 项目名称
project(VimInts VERSION 1.0)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置编译选项
if(MSVC)
    # MSVC 编译器特定选项
    add_compile_options(/W4)
else()
    # GCC/Clang 编译器选项
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
# 包含头文件目录
include_directories(include)
# 添加源文件
set(SOURCES 
    src/main.cpp
    src/editor.cpp
    src/ui.cpp
    src/command.cpp
)
# 创建可执行文件
add_executable(vimints ${SOURCES})
# 如果需要链接额外的库，可以在这里添加
# target_link_libraries(vimints some_library)
# 安装配置（可选）
install(TARGETS vimints DESTINATION bin)
