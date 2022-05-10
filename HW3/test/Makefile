CFLAGS = -g -Wall -fno-stack-protector -nostdlib -I. -I.. -DUSEMINI
LINKFLAGS = -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2
LINKLIBS = -L. -L.. -lmini



.PHONY: all compile_start write1 alarm1 alarm2 alarm3 jmp1 clean

all: write1 alarm1 alarm2 alarm3 jmp1

compile_start: ../start.asm
	yasm -f elf64 -DYASM -D__x86_64__ -DPIC ../start.asm -o start.o

write1: compile_start
	gcc -c $(CFLAGS) write1.c
	ld $(LINKFLAGS) -o write1 write1.o start.o $(LINKLIBS)
	rm write1.o
	rm start.o

alarm1: compile_start
	gcc -c $(CFLAGS) alarm1.c
	ld $(LINKFLAGS) -o alarm1 alarm1.o start.o $(LINKLIBS)
	rm alarm1.o
	rm start.o

alarm2: compile_start
	gcc -c $(CFLAGS) alarm2.c
	ld $(LINKFLAGS) -o alarm2 alarm2.o start.o $(LINKLIBS)
	rm alarm2.o
	rm start.o

alarm3: compile_start
	gcc -c $(CFLAGS) alarm3.c
	ld $(LINKFLAGS) -o alarm3 alarm3.o start.o $(LINKLIBS)
	rm alarm3.o
	rm start.o

jmp1: compile_start
	gcc -c $(CFLAGS) jmp1.c
	ld $(LINKFLAGS) -o jmp1 jmp1.o start.o $(LINKLIBS)
	rm jmp1.o
	rm start.o

clean:
	rm -rf write1 alarm1 alarm2 alarm3 jmp1 *.o