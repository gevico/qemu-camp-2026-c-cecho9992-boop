#include <stdio.h>

unsigned int gcd_asm(unsigned int a, unsigned int b) {
    unsigned int result;
    
    // TODO: 替换下方的TODO段
    
    __asm__ volatile (
        "mov %1, %%eax\n\t"     // 将a存入eax
        "mov %2, %%ebx\n\t"     // 将b存入ebx
        "jmp .L_check\n\t"      // 跳转到检查条件
        
        ".L_loop:\n\t"
        "   xor %%edx, %%edx\n\t"  // 清空edx（除法余数寄存器）
        "   div %%ebx\n\t"         // eax = eax / ebx，edx = eax % ebx
        "   mov %%ebx, %%eax\n\t"  // 将ebx（除数）赋值给eax
        "   mov %%edx, %%ebx\n\t"  // 将余数赋值给ebx，继续迭代
        
        ".L_check:\n\t"
        "   test %%ebx, %%ebx\n\t" // 测试ebx是否为0（test同或，结果影响标志位）
        "   jne .L_loop\n\t"       // 如果ebx≠0，跳回循环继续计算
        
        "mov %%eax, %0"            // 将最终结果（eax）存入输出变量result
        : "=r" (result)            // 输出操作数：result（r表示通用寄存器）
        : "r" (a), "r" (b)         // 输入操作数：a和b
        : "eax", "ebx", "edx"      // 被修改的寄存器（告知编译器这些寄存器被修改）
    );
    
    return result;
}

int main(int argc, char* argv[]) {
    printf("%d\n", gcd_asm(12, 8));
    printf("%d\n", gcd_asm(7, 5));
    return 0;
}