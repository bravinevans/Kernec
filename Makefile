# =============================================================================
# Kernec Build System
# =============================================================================
#
# Project      : Kernec
# Architecture : x86_64
# Build System : GNU Make
#
# Philosophy
#
#   • Keep the build simple.
#   • Build everything automatically.
#   • Fail fast.
#   • Scale without rewriting.
#
# =============================================================================

SHELL := /bin/bash

.DEFAULT_GOAL := all

# -----------------------------------------------------------------------------
# Toolchain
# -----------------------------------------------------------------------------

CC := gcc
LD := ld
AS := gcc

RM := rm -rf
MKDIR := mkdir -p
# -----------------------------------------------------------------------------
# Directories
# -----------------------------------------------------------------------------

BUILD_DIR := build

KERNEL_DIR := kernel

BOOT_DIR := boot

INCLUDE_DIR := include

# -----------------------------------------------------------------------------
# Output
# -----------------------------------------------------------------------------

KERNEL := $(BUILD_DIR)/kernec.elf
# -----------------------------------------------------------------------------
# Compiler Flags
# -----------------------------------------------------------------------------

CFLAGS := \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-pic \
	-fno-pie \
	-fno-builtin \
	-m64 \
	-mcmodel=kernel \
	-Wall \
	-Wextra \
	-I$(INCLUDE_DIR)

ASFLAGS := \
	-m64 \
	-mcmodel=kernel \
	-ffreestanding \
	-fno-pic \
	-fno-pie

LDFLAGS := \
	-T $(BOOT_DIR)/linker.ld \
	-nostdlib
# -----------------------------------------------------------------------------
# Source Files
# -----------------------------------------------------------------------------

C_SOURCES := $(shell find $(KERNEL_DIR) -type f -name "*.c")

ASM_SOURCES := $(shell find $(KERNEL_DIR) -type f -name "*.S")

SOURCES := \
	$(C_SOURCES) \
	$(ASM_SOURCES)

OBJECTS := \
	$(patsubst $(KERNEL_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES)) \
	$(patsubst $(KERNEL_DIR)/%.S,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
# -----------------------------------------------------------------------------
# Build Rules
# -----------------------------------------------------------------------------

$(BUILD_DIR):
	$(MKDIR) $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	@$(MKDIR) $(dir $@)
	@echo " CC  $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.S
	@$(MKDIR) $(dir $@)
	@echo " AS  $<"
	$(AS) $(ASFLAGS) -c $< -o $@
# -----------------------------------------------------------------------------
# Link
# -----------------------------------------------------------------------------

$(KERNEL): $(OBJECTS) | $(BUILD_DIR)
	@echo " LD  $@"
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

# -----------------------------------------------------------------------------
# Targets
# -----------------------------------------------------------------------------

.PHONY: all clean info

all: $(KERNEL)

clean:
	@echo " CLEAN"
	$(RM) $(BUILD_DIR)

info:
	@echo ""
	@echo "========== Kernec Build =========="
	@echo "Compiler : $(CC)"
	@echo "Assembler: $(AS)"
	@echo "Linker   : $(LD)"
	@echo "Kernel   : $(KERNEL)"
	@echo "C files  : $(words $(C_SOURCES))"
	@echo "ASM files: $(words $(ASM_SOURCES))"
	@echo "Objects  : $(words $(OBJECTS))"
