# VimInts - 跨平台命令行文本编辑器
## 简介
VimInts 是一个受 Vim 和 Nano 启发的跨平台命令行文本编辑器，使用 C++ 实现。
## 特性
- 跨平台支持
- 多模式编辑（普通模式、插入模式、命令模式）
- 基本文件操作（打开、保存、另存为）
## 编译和运行
### 依赖
- C++11 兼容编译器（如 GCC、Clang 或 MSVC）
### 编译步骤
```bash
# 克隆仓库
git clone https://github.com/yourusername/vimints.git
cd vimints
# 编译
make
# 运行
./vimints [可选的文件名]
```
## 使用说明
### 模式
- 普通模式（默认）：
  - `i`: 进入插入模式
  - `h/j/k/l`: 光标移动
  - `x`: 删除当前字符
  - `y`: 复制当前行
  - `p`: 粘贴
  - `:`: 进入命令模式
- 插入模式：
  - `ESC`: 返回普通模式
- 命令模式：
  - `w`: 保存文件
  - `q`: 退出编辑器
  - `wq`: 保存并退出
  - `q!`: 强制退出不保存
  - `e [文件名]`: 打开文件
  - `r [旧文本] [新文本]`: 替换文本
- 可视模式：
  - `v`: 字符选择模式
  - `V`: 行选择模式
  - `Ctrl + v`: 块选择模式
## 许可证
MIT 许可证
## 贡献
欢迎提交 Issue 和 Pull Request！

## 构建命令（windows）
```bash
cmake -B build && cmake --build build
```

我不建议windows用户学/用/vscode配置的c++，会很不幸

end
