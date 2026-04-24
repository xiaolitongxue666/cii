/*
 * 第二章 练习 2.5
 *
 * 答案（2.5）：
 * 栈接口可以有多种可行设计。本实现给出一个“有界栈”替代方案：
 *   1) 构造接口改为 `Stack_newWithCapacity(size_t maximum_capacity)`，在创建时固定上限；
 *   2) `Stack_pushBounded` 返回状态码，而不是在满栈时触发未定义行为；
 *   3) `Stack_popBounded` 通过输出参数返回元素，并用状态码区分空栈与参数错误；
 *   4) `Stack_freeWithCapacity` 负责统一释放并置空句柄。
 *
 * 这种接口把“容量约束”和“错误处理”显式化，便于调用方在边界条件下做可预期控制。
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum StackStatusCode {
	STACK_STATUS_OK = 0,
	STACK_STATUS_FULL = 1,
	STACK_STATUS_EMPTY = 2,
	STACK_STATUS_BAD_ARGUMENT = 3
} StackStatusCode;

typedef struct BoundedStack {
	size_t capacity;
	size_t size;
	void **items;
} BoundedStack;

/*
 * 创建有界栈：
 * - capacity 必须大于 0；
 * - 元素数组单独分配，失败时回滚已分配资源。
 */
static BoundedStack *Stack_newWithCapacity(size_t maximum_capacity) {
	BoundedStack *stack_instance;

	if (maximum_capacity == 0)
		return NULL;

	stack_instance = (BoundedStack *)malloc(sizeof(*stack_instance));
	if (stack_instance == NULL)
		return NULL;

	stack_instance->items = (void **)calloc(maximum_capacity, sizeof(stack_instance->items[0]));
	if (stack_instance->items == NULL) {
		free(stack_instance);
		return NULL;
	}

	stack_instance->capacity = maximum_capacity;
	stack_instance->size = 0;
	return stack_instance;
}

/* 释放并清空外部句柄，避免继续误用已释放对象。 */
static void Stack_freeWithCapacity(BoundedStack **stack_instance_ref) {
	if (stack_instance_ref == NULL || *stack_instance_ref == NULL)
		return;
	free((*stack_instance_ref)->items);
	free(*stack_instance_ref);
	*stack_instance_ref = NULL;
}

/* 入栈：容量已满时返回 FULL，由调用方决定后续策略。 */
static StackStatusCode Stack_pushBounded(BoundedStack *stack_instance, void *item_pointer) {
	if (stack_instance == NULL)
		return STACK_STATUS_BAD_ARGUMENT;
	if (stack_instance->size >= stack_instance->capacity)
		return STACK_STATUS_FULL;
	stack_instance->items[stack_instance->size++] = item_pointer;
	return STACK_STATUS_OK;
}

/* 出栈：通过 out 参数返回栈顶元素，空栈时返回 EMPTY。 */
static StackStatusCode Stack_popBounded(BoundedStack *stack_instance, void **out_item_pointer) {
	if (stack_instance == NULL || out_item_pointer == NULL)
		return STACK_STATUS_BAD_ARGUMENT;
	if (stack_instance->size == 0)
		return STACK_STATUS_EMPTY;
	*out_item_pointer = stack_instance->items[--stack_instance->size];
	return STACK_STATUS_OK;
}

int main(void) {
	BoundedStack *stack_instance = Stack_newWithCapacity(2);
	int value_a = 7;
	int value_b = 9;
	int value_c = 11;
	void *out_item_pointer = NULL;

	/* 边界测试：满栈后继续 push，必须得到 FULL。 */
	assert(stack_instance != NULL);
	assert(Stack_pushBounded(stack_instance, &value_a) == STACK_STATUS_OK);
	assert(Stack_pushBounded(stack_instance, &value_b) == STACK_STATUS_OK);
	assert(Stack_pushBounded(stack_instance, &value_c) == STACK_STATUS_FULL);

	/* 顺序测试：后进先出，最后一次 pop 返回 EMPTY。 */
	assert(Stack_popBounded(stack_instance, &out_item_pointer) == STACK_STATUS_OK);
	assert(*(int *)out_item_pointer == 9);
	assert(Stack_popBounded(stack_instance, &out_item_pointer) == STACK_STATUS_OK);
	assert(*(int *)out_item_pointer == 7);
	assert(Stack_popBounded(stack_instance, &out_item_pointer) == STACK_STATUS_EMPTY);

	Stack_freeWithCapacity(&stack_instance);
	assert(stack_instance == NULL);

	printf("ex2_05: bounded-stack alternative interface checks passed.\n");
	return 0;
}
