# 数据结构学习

用 VS Code + Git 刷数据结构。C 语言实现，跟严蔚敏教材顺序对齐。

---

## 一、环境安装（只需做一次）

当前机器**没有 C 编译器**，先装 MSYS2。管理员 PowerShell 里跑：

```powershell
winget install --id MSYS2.MSYS2 -e
```

装完打开 **MSYS2 UCRT64**（开始菜单里搜），执行：

```bash
pacman -Syu                          # 更新，中途可能要关窗口重开
pacman -S --needed mingw-w64-ucrt-x86_64-gcc \
                   mingw-w64-ucrt-x86_64-gdb \
                   mingw-w64-ucrt-x86_64-make
```

然后把 `C:\msys64\ucrt64\bin` 加进系统 PATH：

```powershell
[Environment]::SetEnvironmentVariable(
  "Path",
  [Environment]::GetEnvironmentVariable("Path","Machine") + ";C:\msys64\ucrt64\bin",
  "Machine"
)
```

**关掉所有终端和 VS Code 重开**（PATH 不重启不生效），验证：

```powershell
gcc --version
gdb --version
```

---

## 二、目录结构

```
ds-learning/
├── .vscode/                     # 已经配好了，直接能用
│   ├── tasks.json               # 编译 / 编译并运行
│   ├── launch.json              # gdb 调试（F5）
│   ├── c_cpp_properties.json    # IntelliSense
│   └── settings.json            # UTF-8 + LF
├── include/common.h             # Status / ElemType / OK / ERROR
├── src/
│   ├── ch01_linear_list/        # ✅ 顺序表、单链表（已完成）
│   ├── ch02_stack_queue/
│   ├── ch03_string/
│   ├── ch04_tree/
│   ├── ch05_graph/
│   ├── ch06_search/
│   └── ch07_sort/
├── notes/                       # 学习笔记
└── build/                       # 编译产物，已被 .gitignore 忽略
```

---

## 三、日常怎么用

| 操作 | 快捷键 / 方式 |
|---|---|
| 编译当前文件 | `Ctrl+Shift+B` |
| 编译并运行 | `Ctrl+Shift+B` → 选「编译并运行当前文件」 |
| 调试当前文件 | 打开 .c 文件 → `F5` |
| 单步进入 | `F11` |
| 下一步 | `F10` |
| 看变量 | 左侧「运行和调试」面板，或鼠标悬停 |

> 调试前必须先**保存文件**（`Ctrl+S`）。调试的是 `build/` 里的 exe，源码没存就不会重新编译。

**为什么要用 F5 而不是 `printf` 大法？**
链表的指针错乱、递归的调用栈、数组越界，靠打印能查到天亮。F5 看一眼变量面板 10 秒搞定。

---

## 四、Git 工作流

### 首次配置

GitHub CLI 当前 token 已失效，重新登录：

```bash
gh auth login          # 选 GitHub.com -> HTTPS -> 用浏览器登录
```

核验身份（这条错了提交记录会挂错名字）：

```bash
git config --global user.name   # 应为 wyl91
git config --global user.email
```

### 建远程仓库并推送

```bash
cd ds-learning
git init -b main
git add .
git commit -m "chore: 初始化数据结构学习仓库，含 VS Code 配置与顺序表/链表实现"

gh repo create ds-learning --public --source=. --push
```

`--public` 换成 `--private` 就是私有库。学习仓库建议公有——有 commit 记录，面试时能当作品集。

### 每学一节就提交一次

```bash
git add src/ch02_stack_queue/
git commit -m "feat(ch02): 顺序栈与括号匹配"
git push
```

**提交信息规范**（学会这个，以后做项目直接受益）：

```
feat(ch01): 新增顺序表合并算法
fix(ch02): 修正循环队列判满条件，应为 (rear+1)%MAXSIZE==front
docs(ch04): 补充二叉树非递归后序遍历笔记
refactor(ch01): 链表插入抽出 GetElem 复用
```

### 分支策略

学习阶段不需要复杂分支。但练一下这个流程对以后有用：

```bash
git switch -c feat/avl-tree     # 开新分支做 AVL
# ...写代码...
git add . && git commit -m "feat(ch04): AVL 四种旋转"
git switch main
git merge feat/avl-tree
git push
```

---

## 五、学习进度

- [x] Ch01 线性表 — 顺序表 `SqList`
- [x] Ch01 线性表 — 单链表 `LinkList`（含就地逆置、快慢指针）
- [ ] Ch02 栈和队列
- [ ] Ch03 串（KMP）
- [ ] Ch04 树与二叉树
- [ ] Ch05 图
- [ ] Ch06 查找
- [ ] Ch07 排序

---

## 六、配套练习

写完一个结构，立刻去刷对应的题。**光写不刷等于没学。**

| 章节 | 题目 |
|---|---|
| 线性表 | LeetCode 206 反转链表、141 环形链表、21 合并两个有序链表 |
| 栈队列 | LeetCode 20 有效括号、232 用栈实现队列、739 每日温度 |
| 串 | LeetCode 28 找出字符串第一个匹配项、459 重复的子字符串 |
| 树 | LeetCode 144/94/145 三种遍历、102 层序遍历、104 最大深度、98 验证BST |
| 图 | LeetCode 200 岛屿数量、207 课程表、743 网络延迟时间 |
| 查找 | LeetCode 704 二分查找、33 搜索旋转排序数组 |
| 排序 | LeetCode 912 排序数组、215 数组中的第K个最大元素 |

---

## 七、两个当场要改的坏习惯

1. **宏名必须全大写**：`Maxsize` → `MAXSIZE`。这是 C 的通用约定，看代码的人一眼就知道是宏。
2. **`SqlList` 拼错了**，正确的是 `SqList`。Sq 是 **Sq**uence（序列）的缩写，跟 SQL 没关系。命名错了以后搜索、找资料全都对不上。
