# 第一章练习（`practice/`）

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
make clean
```

编译器由 `Makefile` 中的 `PRAC_CC` 选择（Git Bash / MinGW 下若 `CC=cc` 会改用 `gcc`）。

## 各题说明

| 目标 | 源文件 | 说明 |
|------|--------|------|
| `ex1-01` | `chapter01/ex1_01_linenum.c` | 笔答写在文件头注释；保持 `CH1_EX1_1_INCREMENT_AFTER_WORD_ENDS_WITH_NL == 0`（C11 `_Static_assert`）。 |
| `ex1-02` | `chapter01/ex1_02_doubleword.c` | `CH1_EX1_2_MODE`：`0` 演示原版相邻相等对计数；`1` 启用修复版自测（连续段只报一次）。 |
| `ex1-03` | `chapter01/ex1_03_strcpy.c` | 两种 `strcpy` 风格实现；`CH1_EX1_3_COMPLETE` 为 `0` 时编译仍过、运行会因未拷贝而断言失败（用于自测未完成状态）。 |

## 参考

- 仓库根目录 `examples/double.c`（`getword`、重复词检测）
