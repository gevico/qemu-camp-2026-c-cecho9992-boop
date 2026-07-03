#include <stdio.h>
#include <limits.h>

#define CHECK_OVERFLOW(carry) \
    (carry ? "Overflow" : "Not Overflow")

int check_add_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;

    __asm__ volatile(
        "addl %%ebx, %%eax\n\t"
        "setc %0\n"
        : "=r"(carry)
        : "a"(a), "b"(b)
        : "cc"
    );

    return carry;
}

int check_sub_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;

    __asm__ volatile(
        "subl %%ebx, %%eax\n\t"
        "setc %0\n"
        : "=r"(carry)
        : "a"(a), "b"(b)
        : "cc"
    );

    return carry;
}

int check_mul_overflow_asm(unsigned int a, unsigned int b) {
    unsigned int high;

    __asm__ volatile(
        "mull %%ebx\n\t"
        "movl %%edx, %0\n"
        : "=r"(high)
        : "a"(a), "b"(b)
        : "edx", "cc"
    );

    return high != 0;
}

int check_div_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char is_zero;

    __asm__ volatile(
        "test %%ebx, %%ebx\n\t"
        "sete %0\n"
        : "=r"(is_zero)
        : "b"(b)
        : "cc"
    );

    return is_zero;
}

int main() {
    printf("(UINT_MAX + 1)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(UINT_MAX, 1)));
    printf("(1, 0)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(1, 0)));

    printf("(0, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(0, 1)));
    printf("(2, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(2, 1)));

    printf("(UINT_MAX, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(UINT_MAX, 2)));
    printf("(1, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(1, 2)));

    printf("(10, 0)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(10, 0)));
    printf("(2, 1)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(2, 1)));

    return 0;
}