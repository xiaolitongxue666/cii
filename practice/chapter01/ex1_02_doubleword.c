/*
 * 第一章 练习 1.2（参考 examples/double.c）
 *
 * 题目：当输入中出现三个或更多相同单词时，原版 double 会输出什么？修改逻辑以去掉该「特性」。
 *
 * 原版行为：对每个「相邻相等」对都打印一行，故 n 个连续相同词共打印 (n-1) 行。
 * 修复目标：每个极大连续相等段只报告一次（在连续段内第一次发现重复时打印）。
 *
 * 练习开关：
 *   CH1_EX1_2_MODE == 0：仅演示原版统计（自测通过，不要求改代码）。
 *   CH1_EX1_2_MODE == 1：启用 doubleword_fixed 的自测；需补全下方标记为 STUDENT 的条件/语句，
 *                        使「a a a」整段只产生 1 次报告，然后重新编译并运行 ./ex102。
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* 设为 1：启用 STUDENT 修复分支与 self_test；设为 0：仅跑原版计数演示 */
#define CH1_EX1_2_MODE 1

int linenum;

int getword(FILE *fp, char *buf, int size) {
	int c;
	c = getc(fp);
	for (; c != EOF && isspace(c); c = getc(fp))
		if (c == '\n')
			linenum++;
	{
		int i = 0;
		for (; c != EOF && !isspace(c); c = getc(fp))
			if (i < size - 1)
				buf[i++] = tolower((unsigned char)c);
		if (i < size)
			buf[i] = '\0';
	}
	if (c != EOF)
		ungetc(c, fp);
	return buf[0] != '\0';
}

/* 原版 doubleword：每个相邻相等对都打印 */
static int doubleword_original_print_count(char *name, FILE *fp) {
	char prev[128], word[128];
	int count = 0;
	linenum = 1;
	prev[0] = '\0';
	while (getword(fp, word, (int)sizeof word)) {
		if (isalpha((unsigned char)word[0]) && strcmp(prev, word) == 0) {
			(void)name;
			count++;
		}
		strcpy(prev, word);
	}
	return count;
}

/*
 * 修复版：每个连续相等段只打印一次。
 * CH1_EX1_2_MODE==1 时：请去掉「STUDENT」处的占位并补全 in_dup_streak 的更新逻辑。
 */
static int doubleword_fixed_print_count(FILE *fp) {
	char prev[128], word[128];
	int count = 0;
	int in_dup_streak = 0;

	linenum = 1;
	prev[0] = '\0';
	rewind(fp);
	while (getword(fp, word, (int)sizeof word)) {
		if (isalpha((unsigned char)word[0]) && strcmp(prev, word) == 0) {
#if CH1_EX1_2_MODE == 1
			/* 连续相等段内只在第一次相邻相等时计数，避免 a a a 打出两行 */
			if (!in_dup_streak) {
				count++;
				in_dup_streak = 1;
			}
#else
			if (!in_dup_streak) {
				count++;
				in_dup_streak = 1;
			}
#endif
		} else {
			in_dup_streak = 0;
		}
		strcpy(prev, word);
	}
	return count;
}

#if CH1_EX1_2_MODE == 1
static void self_test(void) {
	FILE *fp = tmpfile();
	assert(fp != NULL);
	assert(fputs("a a a\n", fp) >= 0);
	assert(fflush(fp) == 0);
	rewind(fp);
	assert(doubleword_fixed_print_count(fp) == 1);
	fclose(fp);
}
#endif

int main(void) {
	FILE *fp = tmpfile();
	assert(fp != NULL);
	assert(fputs("a a a\n", fp) >= 0);
	assert(fflush(fp) == 0);
	rewind(fp);
	{
		int n = doubleword_original_print_count(NULL, fp);
		printf("ex1_02: original count for \"a a a\" (adjacent equal pairs) = %d (expected 2, n-1)\n", n);
		assert(n == 2);
	}
#if CH1_EX1_2_MODE == 0
	rewind(fp);
	{
		int k = doubleword_fixed_print_count(fp);
		printf("ex1_02: fixed version count for same input (expect 1) = %d\n", k);
		assert(k == 1);
	}
#endif
#if CH1_EX1_2_MODE == 1
	self_test();
	printf("ex1_02: CH1_EX1_2_MODE==1 self-test passed.\n");
#else
	printf("ex1_02: set CH1_EX1_2_MODE to 1 and fix STUDENT block per comments to finish.\n");
#endif
	return 0;
}
