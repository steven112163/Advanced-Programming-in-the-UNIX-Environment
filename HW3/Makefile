ASSEMBLYFLAGS = -f elf64 -DYASM -D__x86_64__ -DPIC
CFLAGS = -g -Wall -fno-stack-protector -fPIC -nostdlib

BUILD = build



.PHONY: all clean

all: libmini64.asm libmini.c
	mkdir -p $(BUILD)
	yasm $(ASSEMBLYFLAGS) libmini64.asm -o $(BUILD)/libmini64.o
	gcc -o $(BUILD)/libmini.o -c $(CFLAGS) libmini.c
	ld -shared -o libmini.so $(BUILD)/libmini64.o $(BUILD)/libmini.o

clean:
	rm -rf libmini.so $(BUILD)