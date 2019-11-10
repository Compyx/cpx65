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
	      -Wredundant-decls -Wnested-externs -Wunreachable-code



BIN_ASM = cpx65asm
BIN_DISASM = cpx65dis
BIN_LD = cpx65ld
BIN_TESTS = unit_tests

all: $(BIN_ASM) $(BIN_TESTS)


BASE_OBJS = addrmode.o cputype.o mnemonic.o opcode.o cmdline.o strlist.o mem.o \
	    error.o objpool.o binfile.o

TEST_OBJS = unit.o test_cpu.o test_mem.o test_objpool.o test_io.o \
	    test_unittest.o


$(BIN_ASM): src/asm/main.o $(BASE_OBJS)
	$(LD) -o $@ $^

$(BIN_TESTS): src/tests/unit_tests.o $(BASE_OBJS) $(TEST_OBJS)
	$(LD) -o $@ $^



%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<




.PHONY: clean
clean:
	rm -f *.o
	rm -f $(BASE_OBJS) $(TEST_OBJS)
	rm -f $(BIN_ASM) $(BIN_DISAM) $(BIN_LD) $(BIN_TESTS)



