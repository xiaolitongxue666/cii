# 练习代码（`practice/`）

与仓库根目录 `examples/` 中示例对应的笔答与小实验，可在本目录单独编译、运行。

## 构建与运行

在仓库根目录：

```sh
make practice
```

在本目录：

```sh
make              # 或 make all
make run-ex1-01   # 运行 build/ex1-01（Windows 为 .exe）
make run-ex1-02
make run-ex1-03
make run-ex2-01
make run-ex2-02
make run-ex2-03
make run-ex2-04
make run-ex2-05
make clean
```

编译器由 `Makefile` 中的 `PRAC_CC` 选择（Git Bash / MinGW 下若 `CC=cc` 会改用 `gcc`）。

## 第一章（chapter01）说明

| 目标 | 源文件 | 说明 |
|------|--------|------|
| `ex1-01` | `chapter01/ex1_01_linenum.c` | 笔答写在文件头注释；保持 `CH1_EX1_1_INCREMENT_AFTER_WORD_ENDS_WITH_NL == 0`（C11 `_Static_assert`）。 |
| `ex1-02` | `chapter01/ex1_02_doubleword.c` | `CH1_EX1_2_MODE`：`0` 演示原版相邻相等对计数；`1` 启用修复版自测（连续段只报一次）。 |
| `ex1-03` | `chapter01/ex1_03_strcpy.c` | 两种 `strcpy` 风格实现；`CH1_EX1_3_COMPLETE` 为 `0` 时编译仍过、运行会因未拷贝而断言失败（用于自测未完成状态）。 |

## 第二章（chapter02）说明

| 目标 | 源文件 | 说明 |
|------|--------|------|
| `ex2-01` | `chapter02/ex2_01_divtest.c` | 比较“条件编译配置”与“显式表达式测试”两种语义判定方式；2.1 答案在文件头注释。 |
| `ex2-02` | `chapter02/ex2_02_cross_arith.c` | 运行时探测除法语义并修正 `div/mod`，避免交叉编译时编译期常量折叠造成偏差；2.2 答案在文件头注释。 |
| `ex2-03` | `chapter02/ex2_03_stack_guard.c` | 通过 `magic` 签名检测部分外来 `Stack_T` 误用，入口统一断言校验；2.3 答案在文件头注释。 |
| `ex2-04` | `chapter02/ex2_04_badptr.c` | `isBadPtr` 宏示例（空指针+对齐检查），并注明其“仅部分检测”的边界；2.4 答案在文件头注释。 |
| `ex2-05` | `chapter02/ex2_05_stack_alt.c` | 有界栈接口替代方案：创建时指定容量，`push/pop` 返回状态码；2.5 答案在文件头注释。 |

## 参考

- 仓库根目录 `examples/double.c`（`getword`、重复词检测）
