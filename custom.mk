# 部分 Make（如 MinGW）会把 include 进来的本文件中首个目标当作默认目标。
.DEFAULT_GOAL := all

# 根目录执行 make practice 会构建 practice/ 下第一章练习（ex101–ex103）。
.PHONY: practice
practice:
	$(MAKE) -C practice $(if $(strip $(BUILDDIR)),BUILDDIR=$(BUILDDIR),)
