/*
 * 第二章 练习 2.4
 *
 * 答案（2.4）：
 * 可定义一个“系统相关”的 `isBadPtr(p)` 宏，把可判定的非法指针情况先拦住，例如：
 *   1) `p == NULL`；
 *   2) 地址不满足目标类型对齐约束（如 double 常见需要 8 字节对齐）。
 *
 * 本文件实现了该思路：用 `uintptr_t` 做按位对齐检查，并在 `isBadPtr` 中组合“空指针 + 对齐”。
 * 这类检查是保守检查，只能识别一部分坏指针；对“地址在进程空间外/已释放/无权限页”等情况，
 * 需要平台 API 才能做更强检测，因此题目要求它是 system-specific macro。
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

/*
 * 对齐检查：如果目标对象要求 alignment 字节对齐，而地址不满足，则视为坏指针。
 * 这是“可移植且可判定”的子集检查，但不能作为唯一依据：
 *   - 对齐正确并不代表指针一定有效（可能悬空、越界、已释放、无访问权限）。
 *   - 因此 isBadPtr 在这里是“保守过滤器”，用于拦截一部分明显坏指针。
 *   - 对更强检测需依赖系统机制（平台 API、运行时检测工具等）。
 */
#define PTR_MISALIGNED_FOR(ptrValue, alignment) \
	((((uintptr_t)(const void *)(ptrValue)) & ((uintptr_t)(alignment) - 1u)) != 0u)

/*
 * 系统特定扩展点：
 * - 可在这里按平台接入更强的地址空间探测机制。
 * - 当前默认只做“空指针 + 对齐”检查，避免伪造跨平台可靠性。
 * - 这里选用 8 字节作为 double 常见对齐基准，便于演示该练习题语境。
 */
#if defined(_WIN32)
#define isBadPtr(pointerValue) ((pointerValue) == NULL || PTR_MISALIGNED_FOR((pointerValue), 8u))
#elif defined(__linux__)
#define isBadPtr(pointerValue) ((pointerValue) == NULL || PTR_MISALIGNED_FOR((pointerValue), 8u))
#else
#define isBadPtr(pointerValue) ((pointerValue) == NULL || PTR_MISALIGNED_FOR((pointerValue), 8u))
#endif

int main(void) {
	double aligned_value = 0.0;
	unsigned char buffer[32];
	double *aligned_pointer = &aligned_value;
	/* 人工构造“非 8 字节对齐”的地址，作为坏指针样例。 */
	double *misaligned_pointer = (double *)(void *)(buffer + 1);

	/* 自测覆盖：合法对齐、空指针、错位地址。 */
	assert(!isBadPtr(aligned_pointer));
	assert(isBadPtr(NULL));
	assert(isBadPtr(misaligned_pointer));

	printf("ex2_04: isBadPtr scaffold checks passed.\n");
	return 0;
}
