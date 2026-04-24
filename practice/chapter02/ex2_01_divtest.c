/*
 * 第二章 练习 2.1
 *
 * 目标：构造一个“实验框架”，比较两类判定方式在可维护性和可移植性上的差异：
 *   1) 预处理器/条件编译方式（编译前选择路径）
 *   2) 显式表达式测试方式（代码里直接写可执行语义判定）
 *
 * 答案（2.1）：
 * 与预处理器宏/条件编译相比，显式测试 `-13/5 == -2` 更好，核心原因有三点：
 * 1) 语义自证：测试直接表达“当前实现的整数除法语义”，读代码即可看到判定依据，
 *    不依赖外部构建参数或平台宏定义是否正确。
 * 2) 维护成本低：同一份源码可在不同实现上编译运行，不需要为不同编译器/平台维护多套
 *    `#if` 分支，减少配置漂移导致的错误。
 * 3) 防误配更强：条件编译依赖“预先知道并正确声明语义”；一旦宏配置错误会静默走错分支。
 *    显式测试则由表达式结果驱动，更接近“按事实选择行为”。
 *
 * 注：本题聚焦“用代码内显式测试替代条件编译配置”的优势；2.2 会进一步处理交叉编译场景。
 */

#include <assert.h>
#include <stdio.h>

/*
 * 模式：
 *   0 => 走“编译期模拟配置”路径
 *   1 => 走“显式表达式测试”路径
 */
#ifndef CH2_EX2_1_MODE
#define CH2_EX2_1_MODE 1
#endif

/* 模拟“预处理器配置”（用于对照） */
#ifndef CH2_EX2_1_CFG_TRUNC_TOWARD_ZERO
#define CH2_EX2_1_CFG_TRUNC_TOWARD_ZERO 1
#endif

/*
 * 对照实现 A：依赖外部配置宏描述“当前除法语义”。
 * 风险在于配置与真实运行语义可能不一致。
 */
static int div_from_cfg(int dividend, int divisor) {
	int quotient = dividend / divisor;
	int remainder = dividend % divisor;

	if (!CH2_EX2_1_CFG_TRUNC_TOWARD_ZERO && (dividend < 0) != (divisor < 0) && remainder != 0)
		return quotient - 1;
	return quotient;
}

/*
 * 对照实现 B：代码内显式测试语义，再决定是否修正为 floor 除法。
 */
static int div_from_explicit_test(int dividend, int divisor) {
	int quotient = dividend / divisor;
	int remainder = dividend % divisor;

	if (-13 / 5 == -2 && (dividend < 0) != (divisor < 0) && remainder != 0)
		return quotient - 1;
	return quotient;
}

static void run_case(int dividend, int divisor) {
	int value_from_cfg = div_from_cfg(dividend, divisor);
	int value_from_explicit = div_from_explicit_test(dividend, divisor);

#if CH2_EX2_1_MODE == 0
	printf("mode=cfg      x=%d y=%d result=%d\n", dividend, divisor, value_from_cfg);
#else
	printf("mode=explicit x=%d y=%d result=%d\n", dividend, divisor, value_from_explicit);
#endif

	/* 这里仅保留最小断言，题解见文件头注释。 */
	if ((dividend % divisor) == 0) {
		/* 可整除场景下，不论采用哪种舍入语义，结果都应与原生 / 一致。 */
		assert(value_from_cfg == dividend / divisor);
		assert(value_from_explicit == dividend / divisor);
	}
}

int main(void) {
	/* 覆盖：异号非整除、同号整除两类典型路径。 */
	run_case(-13, 5);
	run_case(13, -5);
	run_case(-12, 3);
	run_case(12, 3);

	printf("ex2_01: experiment scaffold ready; answer inserted in header comments.\n");
	return 0;
}
