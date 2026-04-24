/*
 * 第二章 练习 2.3
 *
 * 答案（2.3）：
 * 为了检测“外来 Stack_T”（非 Stack_new 创建的对象），可在栈对象中增加一个签名字段 magic：
 *   1) Stack_new 创建对象时把 magic 设为固定常量（如 STACK_MAGIC_LIVE）。
 *   2) 在 Stack_empty/Stack_push/Stack_pop/Stack_free 入口统一校验：
 *      指针非空且 magic 正确，否则触发断言。
 *   3) Stack_free 释放前把 magic 改成另一个值（如 STACK_MAGIC_FREED），并将外部句柄置 NULL，
 *      以便尽早发现重复释放或悬空句柄误用。
 *
 * 这种方法不能捕获所有非法指针，但能覆盖一部分高频错误：伪造对象、已释放对象再次使用、
 * 错误传参导致的非本 ADT 实例混入。
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct StackImplementation *Stack_T;

enum {
	STACK_MAGIC_LIVE = (int)0x534B544B,   /* 'SKTK' */
	STACK_MAGIC_FREED = (int)0x46524545   /* 'FREE' */
};

struct StackElement {
	void *value;
	struct StackElement *next;
};

struct StackImplementation {
	int magic;
	int count;
	struct StackElement *head;
};

/* 统一合法性谓词：非空且带有“活跃对象”签名。 */
static int stack_is_valid(Stack_T stack_instance) {
	return stack_instance != NULL && stack_instance->magic == STACK_MAGIC_LIVE;
}

/* 构造：初始化签名、元素计数和链表头。 */
static Stack_T Stack_new(void) {
	Stack_T stack_instance = (Stack_T)malloc(sizeof(*stack_instance));
	assert(stack_instance != NULL);
	stack_instance->magic = STACK_MAGIC_LIVE;
	stack_instance->count = 0;
	stack_instance->head = NULL;
	return stack_instance;
}

static int Stack_empty(Stack_T stack_instance) {
	assert(stack_is_valid(stack_instance));
	return stack_instance->count == 0;
}

/* 压栈：头插法，时间复杂度 O(1)。 */
static void Stack_push(Stack_T stack_instance, void *value_pointer) {
	struct StackElement *new_element;

	assert(stack_is_valid(stack_instance));
	new_element = (struct StackElement *)malloc(sizeof(*new_element));
	assert(new_element != NULL);
	new_element->value = value_pointer;
	new_element->next = stack_instance->head;
	stack_instance->head = new_element;
	stack_instance->count++;
}

static void *Stack_pop(Stack_T stack_instance) {
	struct StackElement *head_element;
	void *value_pointer;

	assert(stack_is_valid(stack_instance));
	/* 空栈弹出属于接口误用，直接断言。 */
	assert(stack_instance->count > 0);

	head_element = stack_instance->head;
	stack_instance->head = head_element->next;
	stack_instance->count--;
	value_pointer = head_element->value;
	free(head_element);
	return value_pointer;
}

/*
 * 释放：
 * 1) 逐节点释放链表；
 * 2) 将 magic 改为 FREED，帮助调试时尽早暴露重复释放/悬空使用；
 * 3) 外部句柄置 NULL，减少野指针继续传播。
 */
static void Stack_free(Stack_T *stack_instance_ref) {
	struct StackElement *current_element;

	assert(stack_instance_ref != NULL);
	assert(stack_is_valid(*stack_instance_ref));

	current_element = (*stack_instance_ref)->head;
	while (current_element != NULL) {
		struct StackElement *next_element = current_element->next;
		free(current_element);
		current_element = next_element;
	}

	(*stack_instance_ref)->magic = STACK_MAGIC_FREED;
	free(*stack_instance_ref);
	*stack_instance_ref = NULL;
}

int main(void) {
	Stack_T stack_instance = Stack_new();
	int first_value = 11;
	int second_value = 29;

	/* 正常路径自测：LIFO 顺序与空栈状态转换。 */
	assert(Stack_empty(stack_instance));
	Stack_push(stack_instance, &first_value);
	Stack_push(stack_instance, &second_value);
	assert(Stack_empty(stack_instance) == 0);
	assert(*(int *)Stack_pop(stack_instance) == 29);
	assert(*(int *)Stack_pop(stack_instance) == 11);
	assert(Stack_empty(stack_instance));
	Stack_free(&stack_instance);

	/*
	 * 负例说明（默认不触发，避免中断）：
	 * 将下面代码块改为 #if 1 可观察断言拦截效果。
	 */
#if 0
	{
		struct StackImplementation fake_stack = {0, 0, NULL};
		/* 伪造对象未带有效 magic，应被入口断言拦截。 */
		(void)Stack_empty(&fake_stack); /* 应触发断言 */
	}
#endif

	printf("ex2_03: guarded stack checks passed.\n");
	return 0;
}
