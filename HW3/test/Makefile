CFLAGS = -g -Wall -fno-stack-protector -nostdlib -I. -I.. -DUSEMINI
LINKFLAGS = -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2
LINKLIBS = -L. -L.. -lmini

BUILD = build



.PHONY: all compile_start write1 alarm1 alarm2 alarm3 jmp1 clean

all: write1 alarm1 alarm2 alarm3 jmp1

compile_start: ../start.asm
	mkdir -p $(BUILD)
	yasm -f elf64 -DYASM -D__x86_64__ -DPIC ../start.asm -o $(BUILD)/start.o

write1: compile_start
	gcc -c $(CFLAGS) write1.c -o $(BUILD)/write1.o
	ld $(LINKFLAGS) -o write1 $(BUILD)/write1.o $(BUILD)/start.o $(LINKLIBS)

alarm1: compile_start
	gcc -c $(CFLAGS) alarm1.c -o $(BUILD)/alarm1.o
	ld $(LINKFLAGS) -o alarm1 $(BUILD)/alarm1.o $(BUILD)/start.o $(LINKLIBS)

alarm2: compile_start
	gcc -c $(CFLAGS) alarm2.c -o $(BUILD)/alarm2.o
	ld $(LINKFLAGS) -o alarm2 $(BUILD)/alarm2.o $(BUILD)/start.o $(LINKLIBS)

alarm3: compile_start
	gcc -c $(CFLAGS) alarm3.c -o $(BUILD)/alarm3.o
	ld $(LINKFLAGS) -o alarm3 $(BUILD)/alarm3.o $(BUILD)/start.o $(LINKLIBS)

jmp1: compile_start
	gcc -c $(CFLAGS) jmp1.c -o $(BUILD)/jmp1.o
	ld $(LINKFLAGS) -o jmp1 $(BUILD)/jmp1.o $(BUILD)/start.o $(LINKLIBS)

clean:
	rm -rf write1 alarm1 alarm2 alarm3 jmp1 $(BUILD) *.o