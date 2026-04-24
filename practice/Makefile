# 第一章练习：每题可单独编译、单独运行（Git Bash 下用 make 已测）。
#
# 单独编译某一题：
#   make ex1-01
# 编译并运行：
#   make run-ex1-01
# 全部：
#   make all
#
# 说明：Git Bash 里通常既有 gcc 也有 cc；但 make 往往沿用环境里的 CC=cc，
# 而 cc 与 gcc 在部分 MinGW 安装上并非同一套链，cc 可能编链失败。
# 此处用 PRAC_CC：若 CC 为 cc 则改用 gcc，否则沿用 CC（可 make CC=clang）。

BUILDDIR := build

UNAMES := $(shell uname -s 2>/dev/null || echo unknown)
ifneq (,$(findstring MINGW,$(UNAMES)))
EXEEXT := .exe
else ifneq (,$(findstring MSYS,$(UNAMES)))
EXEEXT := .exe
else ifeq ($(OS),Windows_NT)
EXEEXT := .exe
else
EXEEXT :=
endif

PRAC_CC := $(if $(CC),$(CC),gcc)
ifeq ($(PRAC_CC),cc)
PRAC_CC := gcc
endif
CFLAGS ?= -g
LDFLAGS ?=
RM ?= rm -f
MKDIR ?= mkdir -p

define EX_BIN
$(BUILDDIR)/ex$(1)-$(2)$(EXEEXT)
endef

EX1_01 := $(call EX_BIN,1,01)
EX1_02 := $(call EX_BIN,1,02)
EX1_03 := $(call EX_BIN,1,03)
EX2_01 := $(call EX_BIN,2,01)
EX2_02 := $(call EX_BIN,2,02)
EX2_03 := $(call EX_BIN,2,03)
EX2_04 := $(call EX_BIN,2,04)
EX2_05 := $(call EX_BIN,2,05)

.PHONY: all clean \
	ex1-01 ex1-02 ex1-03 ex2-01 ex2-02 ex2-03 ex2-04 ex2-05 \
	run-ex1-01 run-ex1-02 run-ex1-03 run-ex2-01 run-ex2-02 run-ex2-03 run-ex2-04 run-ex2-05

all: ex1-01 ex1-02 ex1-03 ex2-01 ex2-02 ex2-03 ex2-04 ex2-05

ex1-01: $(EX1_01)
ex1-02: $(EX1_02)
ex1-03: $(EX1_03)
ex2-01: $(EX2_01)
ex2-02: $(EX2_02)
ex2-03: $(EX2_03)
ex2-04: $(EX2_04)
ex2-05: $(EX2_05)

$(BUILDDIR):
	$(MKDIR) $@

$(EX1_01): chapter01/ex1_01_linenum.c | $(BUILDDIR)
	$(PRAC_CC) $(CFLAGS) -std=c11 $(LDFLAGS) -o $@ $<

$(EX1_02): chapter01/ex1_02_doubleword.c | $(BUILDDIR)
	$(PRAC_CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(EX1_03): chapter01/ex1_03_strcpy.c | $(BUILDDIR)
	$(PRAC_CC) $(CFLAGS) -Wall $(LDFLAGS) -o $@ $<

$(EX2_01): chapter02/ex2_01_divtest.c | $(BUILDDIR)
	$(PRAC_CC) $(CFLAGS) -Wall -Wextra $(LDFLAGS) -o $@ $<

$(EX2_02): chapter02/ex2_02_cross_arith.c | $(BUILDDIR)
	$(PRAC_CC) $(CFLAGS) -Wall -Wextra $(LDFLAGS) -o $@ $<

$(EX2_03): chapter02/ex2_03_stack_guard.c | $(BUILDDIR)
	$(PRAC_CC) $(CFLAGS) -Wall -Wextra $(LDFLAGS) -o $@ $<

$(EX2_04): chapter02/ex2_04_badptr.c | $(BUILDDIR)
	$(PRAC_CC) $(CFLAGS) -Wall -Wextra $(LDFLAGS) -o $@ $<

$(EX2_05): chapter02/ex2_05_stack_alt.c | $(BUILDDIR)
	$(PRAC_CC) $(CFLAGS) -Wall -Wextra $(LDFLAGS) -o $@ $<

run-ex1-01: $(EX1_01)
	@$(EX1_01)
run-ex1-02: $(EX1_02)
	@$(EX1_02)
run-ex1-03: $(EX1_03)
	@$(EX1_03)
run-ex2-01: $(EX2_01)
	@$(EX2_01)
run-ex2-02: $(EX2_02)
	@$(EX2_02)
run-ex2-03: $(EX2_03)
	@$(EX2_03)
run-ex2-04: $(EX2_04)
	@$(EX2_04)
run-ex2-05: $(EX2_05)
	@$(EX2_05)

clean:
	$(RM) -r $(BUILDDIR)
