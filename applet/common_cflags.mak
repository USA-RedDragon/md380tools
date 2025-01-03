LLVM_ARM_SYSROOT ?= /opt/llvm-arm-sysroot
CFLAGS_SYSROOT := --sysroot $(LLVM_ARM_SYSROOT) -L$(LLVM_ARM_SYSROOT)/lib/newlib

# Architecture-specific flags
CFLAGS := -target arm-none-eabi -mthumb -mcpu=cortex-m4 -mfloat-abi=hard \
          -mfpu=fpv4-sp-d16 -mlittle-endian -fshort-wchar -mno-unaligned-access

# Fix for redefined snprintf
CFLAGS += -fno-builtin-snprintf

# Compile-time Optimization
CFLAGS += -Ofast
# Link-time Optimization
CFLAGS += -fuse-ld=$(shell which ld.lld)

# Warnings
CFLAGS += -Wall -Werror
# Disabled warnings
CFLAGS += -Wno-unused-command-line-argument
