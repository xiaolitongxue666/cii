/*
 * 第二章 练习 2.2
 *
 * 答案（2.2）：
 * 原问题在于：若 arith.c 由交叉编译器在机器 X 上编译给机器 Y，`-13/5 == -2` 这种写法
 * 可能在编译期按 X 的语义被常量折叠，从而与 Y 运行时语义不一致。
 *
 * 修复方法是在“运行时”判定目标机语义，并据此调整 div/mod：
 *   1) 先计算 q=x/y、r=x%y。
 *   2) 运行时检测是否为“向零截断”（示例：`(-13)/5 == -2`，通过 volatile 防止编译期折叠）。
 *   3) 若为向零截断，且 x、y 异号并且 r!=0，则做修正：
 *      - floor_div = q - 1
 *      - floor_mod = r + y
 *   4) 其余情况保持 q、r 不变。
 *
 * 这样不依赖条件编译，也不依赖编译主机语义，能保证交叉编译后二进制在目标机行为正确。
 */

#include <assert.h>
#include <stdio.h>

/*
 * 运行时判定宿主执行语义是否“向零截断”：
 *   若 -13/5 == -2，则为向零截断；
 *   否则按另一分支处理。
 *
 * 这里的关键点是：判定在目标代码运行时发生，不绑定编译器所在机器语义。
 */
static int runtime_truncates_toward_zero(void) {
	volatile int minus_thirteen = -13;
	volatile int five = 5;
	int quotient = minus_thirteen / five;
	/* 仅返回“目标代码当前运行时语义”的观测值。 */
	return quotient == -2;
}

/*
 * 计算 floor 除法：
 * - 先用机器原生 / 和 % 取得商余；
 * - 仅在“向零截断 + 异号 + 非整除”时做一次性修正。
 */
static int arith_div_cross_safe(int dividend, int divisor) {
	int quotient = dividend / divisor;
	int remainder = dividend % divisor;
	int signs_differ = (dividend < 0) != (divisor < 0);

	if (runtime_truncates_toward_zero() && signs_differ && remainder != 0)
		return quotient - 1;
	return quotient;
}

/* 与 arith_div_cross_safe 配套，保证 x = q*y + r 的一致性。 */
static int arith_mod_cross_safe(int dividend, int divisor) {
	int remainder = dividend % divisor;
	int signs_differ = (dividend < 0) != (divisor < 0);

	if (runtime_truncates_toward_zero() && signs_differ && remainder != 0)
		return remainder + divisor;
	return remainder;
}

static void verify_floor_identity(int dividend, int divisor) {
	int quotient = arith_div_cross_safe(dividend, divisor);
	int remainder = arith_mod_cross_safe(dividend, divisor);

	/* 基本代数恒等式必须成立。 */
	assert(divisor != 0);
	assert(dividend == quotient * divisor + remainder);

	/* 余数符号与范围约束：与除数同号，且绝对值小于 |divisor|。 */
	if (divisor > 0) {
		assert(remainder >= 0);
		assert(remainder < divisor);
	} else {
		assert(remainder <= 0);
		assert(remainder > divisor);
	}
}

int main(void) {
	static const int dividends[] = {-13, -12, -11, -1, 0, 1, 11, 12, 13};
	static const int divisors[] = {-5, -3, -2, 2, 3, 5};
	size_t dividend_index;
	size_t divisor_index;

	for (dividend_index = 0; dividend_index < sizeof(dividends) / sizeof(dividends[0]); ++dividend_index) {
		for (divisor_index = 0; divisor_index < sizeof(divisors) / sizeof(divisors[0]); ++divisor_index) {
			/* 穷举小样本，覆盖正负组合与可整除/不可整除路径。 */
			verify_floor_identity(dividends[dividend_index], divisors[divisor_index]);
		}
	}

	printf("ex2_02: cross-safe arithmetic checks passed.\n");
	return 0;
}
