#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

/*
 * 原始协议头（严格字节对齐）
 */
typedef struct {
    uint8_t version_low;
    uint8_t version_high;
    uint16_t length_be;
    uint8_t flags_raw;
} proto_header_raw_t;

/*
 * 位域视图（语义用）
 */
typedef struct {
    unsigned int ver_major : 4;
    unsigned int ver_minor : 4;
    unsigned int length    : 16;
    unsigned int resv      : 3;
    unsigned int flags     : 5;
} proto_header_bits_t;

#pragma pack(pop)

/*
 * 大端转小端
 */
static uint16_t be16_to_cpu(uint16_t be) {
    return (uint16_t)(((be & 0xFF00) >> 8) | ((be & 0x00FF) << 8));
}

int main(void) {
    const uint8_t stream[5] = {0x00, 0x03, 0x00, 0x20, 0x00};

    proto_header_raw_t raw;
    memset(&raw, 0, sizeof(raw));
    memcpy(&raw, stream, sizeof(stream));

    /* ===== 修复点：版本在 version_high ===== */
    uint8_t v = raw.version_high;

    uint8_t ver_major = (v >> 4) & 0x0F;
    uint8_t ver_minor = v & 0x0F;

    /* ===== 长度（大端）===== */
    uint16_t length = be16_to_cpu(raw.length_be);

    /* ===== flags ===== */
    uint8_t flags = raw.flags_raw & 0x1F;
    uint8_t resv  = (raw.flags_raw >> 5) & 0x07;

    /* ===== 位域结构（仅语义）===== */
    proto_header_bits_t view;
    view.ver_major = ver_major;
    view.ver_minor = ver_minor;
    view.length    = length;
    view.resv      = resv;
    view.flags     = flags;

    printf("version:%u.%u, length:%u, flags:0x%02X\n",
           view.ver_major,
           view.ver_minor,
           view.length,
           view.flags);

    return 0;
}