/*
 * 第一章 练习 1.1（参考仓库根目录 examples/double.c 中的 getword）
 *
 * 题目：getword 在「跳过空白直至非空白或 EOF」阶段遇到 '\n' 会递增 linenum，
 * 但在「将单词拷入 buf」之后，若导致单词结束的字符是换行，却不会在此处再递增 linenum。
 * 请说明原因；若在此处也对 linenum 递增，会发生什么？
 *
 * 考点：行号表示「当前词从哪一行开始」；尾随的 '\n' 被 ungetc 留到下一次调用，
 * 在下一轮开头的空白扫描里计数，避免同一换行被计两次或使行号相对词「超前」。
 *
 * --- 笔答（不参与编译，自行在注释中补写）---
 * 原因简述：结束词的字符（含 '\n'）不属于词，留在 c 里并经 ungetc 退回流。
 *   下次 getword 进入第一个 for 时会把该换行当前导空白消费掉，并在那时递增
 *   linenum，这样「跨过一行」只计数一次，且行号与「下一个词从哪一行开始读」一致。
 * 若在此处再递增：同一物理换行会被计两次（此处一次 + 下一轮前导空白又一次），
 *   或使 linenum 相对当前已读完的词「超前」，重复词报错行号会错位。
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>

int linenum;

/*
 * 与 double.c 一致；拆成两个 for 是因为要做两件不同的事，条件相反：
 *
 *   第一个 for：只「吃掉」前导空白，不写入 buf。结束时 c 是第一个非空白字符，
 *   或已是 EOF。换行在这里计入 linenum（表示读指针跨过了行界）。
 *
 *   第二个 for：从当前 c 起读入「一个词」——连续的非空白，写入 buf。结束时 c
 *   是词后的第一个分隔符（空白或 EOF）；该字符不属于词，留在 c 里供后面 ungetc。
 *
 * 若合并成一个循环，无法在同一套条件里既「跳过任意长空白」又「只收集非空白」。
 * 书中步骤名：<scan forward to a nonspace or EOF>，<copy the word into buf[0..size-1]>。
 *
 * 参数约定：
 *   fp   - 输入流，要求可读；
 *   buf  - 输出缓冲区，函数会保证写入 '\0' 结尾；
 *   size - 缓冲区容量，建议 >= 2（至少容纳一个字符和终止符）。
 *
 * 返回值：
 *   1 表示成功读到一个非空单词；
 *   0 表示遇到 EOF 且本次未形成单词。
 */
int getword(FILE *fp, char *buf, int size) {
	int c;
	c = getc(fp);
	/* 第一个 for：前导空白扫描（isspace 为真则继续读）；遇 '\n' 递增 linenum */
	for (; c != EOF && isspace(c); c = getc(fp))
		if (c == '\n')
			linenum++;
	{
		int i = 0;
		/*
		 * 第二个 for：单词体扫描（!isspace 则属于词）。
		 * - i < size - 1 时才写入，避免溢出；
		 * - 退出时 c 为 EOF 或词尾空白，不属于单词，后续可能回推。
		 */
		for (; c != EOF && !isspace(c); c = getc(fp))
			if (i < size - 1)
				buf[i++] = tolower((unsigned char)c);
		if (i < size)
			buf[i] = '\0';
	}
	/* 回推“分隔符字符”，让下一次调用从正确边界继续。 */
	if (c != EOF)
		ungetc(c, fp);
	return buf[0] != '\0';
}

/*
 * 可编译判题：教材结论为「单词以换行结束时，不应在拷贝阶段之后额外递增 linenum」。
 * 若你认为应递增，将下方宏改为 1 再编译，会触发静态断言失败。
 */
#define CH1_EX1_1_INCREMENT_AFTER_WORD_ENDS_WITH_NL 0

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(CH1_EX1_1_INCREMENT_AFTER_WORD_ENDS_WITH_NL == 0,
	"ex1.1: macro must stay 0 (do not bump linenum after word ends at newline); see book");
#endif

int main(void) {
	char buf[32];
	FILE *fp = tmpfile();

	/* 最小行为自测：输入 "hi\\n" 时，能够稳定读取一个词且流程无断言失败。 */
	assert(fp != NULL);
	assert(fputs("hi\n", fp) >= 0);
	assert(fflush(fp) == 0);
	rewind(fp);
	linenum = 1;
	(void)getword(fp, buf, (int)sizeof buf);
	fclose(fp);
	printf("ex1_01: macro check passed; write short answers in top-of-file comments.\n");
	return 0;
}
