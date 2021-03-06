# vim: noet ts=8 sw=8 sts=8:
#
# Makefile for cpx65

VPATH = src:src/base:src/base/cpu:src/base/io:src/tests

CC = gcc
LD = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g \
	 -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align \
	 -Wstrict-prototypes -Wmissing-prototypes \
	 -Wswitch-default -Wswitch-enum -Wuninitialized -Wconversion \
	 -Wredundant-decls -Wnested-externs -Wunreachable-code \
	 -DHAVE_DEBUG



BIN_ASM = cpx65as
BIN_DISASM = cpx65da
BIN_LD = cpx65ld
BIN_TESTS = unit_tests

all: $(BIN_ASM) $(BIN_DISASM) $(BIN_TESTS)

# objects in src/base/cpu
BASE_CPU_OBJS = addrmode.o cputype.o mnemonic.o opcode.o

# objects in src/base/io
BASE_IO_OBJS = binfile.o txtfile.o

# objects in src/base and its subdirs
BASE_OBJS = cmdline.o strlist.o mem.o error.o objpool.o strpool.o symtab.o \
	    $(BASE_CPU_OBJS) \
	    $(BASE_IO_OBJS)

# objects in src/tests
TEST_OBJS = unit.o test_cpu.o test_mem.o test_objpool.o test_io.o \
	    test_symtab.o test_strpool.o \
	    test_unittest.o


$(BIN_ASM): src/asm/main.o $(BASE_OBJS)
	$(LD) -o $@ $^

$(BIN_DISASM): src/disasm/main.o $(BASE_OBJS)
	$(LD) -o $@ $^

$(BIN_TESTS): src/tests/unit_tests.o $(BASE_OBJS) $(TEST_OBJS)
	$(LD) -o $@ $^



%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<




.PHONY: clean
clean:
	rm -f *.o
	rm -f $(BASE_OBJS) $(TEST_OBJS)
	rm -f $(BIN_ASM) $(BIN_DISASM) $(BIN_LD) $(BIN_TESTS)


.PHONY: doc
doc:
	doxygen

