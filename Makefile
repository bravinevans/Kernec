# =============================================================================
# Kernec Build System
# =============================================================================
#
# Project      : Kernec
# Build System : GNU Make
# Architecture : x86_64
#
# This Makefile is the single entry point for building Kernec.
#
# Philosophy:
#   - Keep the build system simple.
#   - Grow only when required.
#   - Fail early.
#   - Keep responsibilities separated.
#
# =============================================================================

SHELL := /bin/bash

# -------------------------------------------------------------------------
# Toolchain
# -------------------------------------------------------------------------

CC      := gcc
LD      := ld
AS      := gcc

# -------------------------------------------------------------------------
# Directories
# -------------------------------------------------------------------------

BUILD_DIR := build

BOOT_DIR := boot

KERNEL_DIR := kernel

INCLUDE_DIR := include

# -------------------------------------------------------------------------
# Output
# -------------------------------------------------------------------------

KERNEL := $(BUILD_DIR)/kernec.elf

# -------------------------------------------------------------------------
# Compiler Flags
# -------------------------------------------------------------------------

CFLAGS := \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-pic \
    -fno-builtin \
    -Wall \
    -Wextra \
    -I$(INCLUDE_DIR)

LDFLAGS := \
    -T $(BOOT_DIR)/kernel.ld \
    -nostdlib

# -------------------------------------------------------------------------
# Sources
# -------------------------------------------------------------------------

C_SOURCES := $(shell find $(KERNEL_DIR) -name "*.c")

ASM_SOURCES := $(shell find $(KERNEL_DIR) -name "*.S")

OBJECTS := \
$(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES)) \
$(patsubst %.S,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

OBJS := $(BUILD_DIR)/boot.o
OBJS += $(BUILD_DIR)/serial.o
OBJS += $(BUILD_DIR)/cpu.o
OBJS += $(BUILD_DIR)/panic.o
OBJS += $(BUILD_DIR)/main.o

# -------------------------------------------------------------------------
# Targets
# -------------------------------------------------------------------------

.PHONY: all clean info

all:
    @echo ""
    @echo "========================================"
    @echo "        Kernec Build System"
    @echo "========================================"
    @echo ""

    @if [ -z "$(C_SOURCES)$(ASM_SOURCES)" ]; then \
        echo "No kernel source files found."; \
        echo "Build system verified."; \
        exit 0; \
    fi

    @echo "Kernel sources detected."
    @echo "Compilation stage will be added in Sprint 3."

$(BUILD_DIR)/cpu.o: kernel/arch/x86_64/cpu.c include/kernec/cpu.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/panic.o: kernel/core/panic.c include/kernec/panic.h include/kernec/serial.h include/kernec/cpu.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -rf $(BUILD_DIR)

info:
    @echo "Compiler : $(CC)"
    @echo "Linker   : $(LD)"
    @echo "Kernel   : $(KERNEL)"
