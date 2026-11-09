/* Install a private kernel GDT before loading the IDT. */
#include <kernec/gdt.h>

#include <stdint.h>

struct gdt_pointer {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

/* Null, 64-bit ring-0 code, and ring-0 data descriptors. */
static const uint64_t gdt[] __attribute__((aligned(16))) = {
    0x0000000000000000ull,
    0x00AF9A000000FFFFull,
    0x00CF92000000FFFFull,
};

extern void gdt_load(const struct gdt_pointer *pointer);

void gdt_init(void)
{
    const struct gdt_pointer pointer = {
        .limit = (uint16_t)(sizeof(gdt) - 1u),
        .base = (uint64_t)(uintptr_t)&gdt[0],
    };

    gdt_load(&pointer);
}
