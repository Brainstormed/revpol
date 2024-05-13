#include <stdio.h>
#include <stdlib.h>
#define MAX_BUF_SZ 1024
#define LOOP_UP_LIM 2048
#define TOINT(str) ((int)(strtol(str, NULL, 10)))

void push(int* stack, int* stack_sz, int num)
{
    if (*stack_sz < MAX_BUF_SZ) {
        stack[(*stack_sz)++] = num;
    } else {
        fprintf(stderr, "Error! Stack overflow!\n");
        exit(EXIT_FAILURE);
    }
}

int pop(int* stack, int* stack_sz)
{
    if (*stack_sz > 0) {
        return stack[--(*stack_sz)];
    } else {
        fprintf(stderr, "Error! Stack underflow!\n");
        exit(EXIT_FAILURE);
    }
}

void parse_op(const char** raw_dat, int raw_sz, int* stack, int* stack_sz)
{
    size_t i = 0;
    size_t loop_cnt = 0;
    int num1 = 0;
    int num2 = 0;

    while (raw_sz-- > 0) {
        if (loop_cnt++ > LOOP_UP_LIM) {
            fprintf(stderr, "Invalid expression!\n"
                            "For multiplication use 'x' or '\\*'\n");
            exit(EXIT_FAILURE);
        }

        switch (*(raw_dat + i++)[0]) {
        case '+':
            num1 = pop(stack, stack_sz);
            num2 = pop(stack, stack_sz);
            push(stack, stack_sz, num1 + num2);
            break;

        case '-':
            num1 = pop(stack, stack_sz);
            num2 = pop(stack, stack_sz);
            push(stack, stack_sz, num2 - num1);
            break;

        case 'x':
        case '*':
            num1 = pop(stack, stack_sz);
            num2 = pop(stack, stack_sz);
            push(stack, stack_sz, num1 * num2);
            break;

        case '/':
            num1 = pop(stack, stack_sz);
            num2 = pop(stack, stack_sz);

            if (num1 != 0) {
                push(stack, stack_sz, num2 / num1);
            } else {
                fprintf(stderr, "Cannot divide by zero!\n");
                exit(EXIT_FAILURE);
            }
            break;

        case '%':
            num1 = pop(stack, stack_sz);
            num2 = pop(stack, stack_sz);

            if (num1 != 0) {
                push(stack, stack_sz, num2 % num1);
            } else {
                fprintf(stderr, "Cannot divide by zero!\n");
                exit(EXIT_FAILURE);
            }
            break;

        default:
            push(stack, stack_sz, TOINT(*(raw_dat + i - 1)));
            break;
        }
    }
}

int main(int argc, const char** argv)
{
    if (argc <= 1) {
        fprintf(stderr, "No expression provided!\n"
                        "Use reverse polish notation\n"
                        "For multiplication use 'x' or '\\*'\n");
        exit(EXIT_FAILURE);
    }

    int stack[MAX_BUF_SZ];
    int stack_sz = 0;

    parse_op(argv + 1, argc - 1, stack, &stack_sz);

    printf("%d\n", *stack);
    return EXIT_SUCCESS;
}
