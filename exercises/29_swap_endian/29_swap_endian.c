#include <stdint.h>
#include <stdio.h>

uint32_t swap_endian(uint32_t num) {
    // TODO: 在这里添加你的代码
    uint8_t byte1 = (num >> 24) & 0xFF;  // 最高位字节（第4字节）
    uint8_t byte2 = (num >> 16) & 0xFF;  // 第3字节
    uint8_t byte3 = (num >> 8) & 0xFF;   // 第2字节
    uint8_t byte4 = num & 0xFF;          // 最低位字节（第1字节）
    
    // 重新组合：将拆分的字节反向移位后合并
    return (uint32_t)byte4 << 24 | 
           (uint32_t)byte3 << 16 | 
           (uint32_t)byte2 << 8  | 
           byte1;
}

int main(int argc, char* argv[]) {
    uint32_t num = 0x78563412;
    uint32_t swapped = swap_endian(num);
    printf("0x%08x -> 0x%08x\n", num, swapped);
    return 0;
}