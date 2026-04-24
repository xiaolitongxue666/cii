/*
 * 第一章 练习 1.3
 *
 * 题目：许多 C 程序员在 strcpy 循环中写显式与 '\0' 的比较，以避免「赋值写在条件里」被误读为笔误；
 * 部分静态分析工具（如 PC-Lint、LCLint）会对「赋值结果用作条件」告警。
 *
 * 练习：
 *   1) 实现下面两个版本，使 main 中的 memcmp 校验通过。
 *   2) 将 CH1_EX1_3_COMPLETE 改为 0 后编译仍应通过；运行 ./ex103 会因未真正拷贝而断言失败。
 *   3)（可选）用 gcc -Wall 编译本文件，对比两种写法是否触发 -Wparentheses / 相关告警。
 *
 * 将宏 CH1_EX1_3_COMPLETE 保持为 1 表示你已完成两个函数（仓库默认如此，便于一键构建）。
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifndef CH1_EX1_3_COMPLETE
#define CH1_EX1_3_COMPLETE 1
#endif

#if CH1_EX1_3_COMPLETE

/*
 * 两版语义相同：逐字节复制直至含终止符；区别仅在条件是否显式写出 != '\0'。
 * -Wall 下若误写成 == 易触发 -Wparentheses；此处为赋值再判真假，属有意为之。
 * 两个函数都返回 dst 的起始地址，行为与标准 strcpy 保持一致。
 */
/* 常见习惯：依赖「赋值为 0 时 while 结束」，不显式写 '\0' */
static char *strcpy_assign_in_cond(char *dst, const char *src) {
	char *s = dst;
	/* 当复制到 '\0' 时，表达式值为 0，循环自然结束。 */
	while ((*dst++ = *src++))
		;
	return s;
}

/* 教材建议：显式写出与 '\0' 的比较，强调赋值并非笔误 */
static char *strcpy_explicit_compare(char *dst, const char *src) {
	char *s = dst;
	/* 与上面完全等价，但可读性对某些团队更友好。 */
	while ((*dst++ = *src++) != '\0')
		;
	return s;
}

#else

static char *strcpy_assign_in_cond(char *dst, const char *src) {
	(void)src;
	(void)dst;
	return dst;
}

static char *strcpy_explicit_compare(char *dst, const char *src) {
	(void)src;
	(void)dst;
	return dst;
}

#endif

int main(void) {
	char a[32], b[32];
	const char *src = "CII chapter 1";

	/* 先清零，便于调试时观察拷贝后的终止符与剩余字节。 */
	memset(a, 0, sizeof a);
	memset(b, 0, sizeof b);
	strcpy_assign_in_cond(a, src);
	strcpy_explicit_compare(b, src);
	/* 逐字节比较（含 '\0'）确保两实现行为一致。 */
	assert(memcmp(a, b, (size_t)strlen(src) + 1u) == 0);
	printf("ex1_03: both strcpy variants match; check passed.\n");
	return 0;
}
