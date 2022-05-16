%macro gensys 2
	global sys_%2:function
sys_%2:
	push	r10
	mov		r10, rcx
	mov		rax, %1
	syscall
	pop		r10
	ret
%endmacro

; RDI, RSI, RDX, RCX, R8, R9

extern errno

	section .data

	section .text

	gensys   0, read
	gensys   1, write
	gensys   2, open
	gensys   3, close
	gensys   9, mmap
	gensys  10, mprotect
	gensys  11, munmap
	gensys  13, rt_sigaction
	gensys  14, rt_sigprocmask
	gensys  22, pipe
	gensys  32, dup
	gensys  33, dup2
	gensys  34, pause
	gensys  35, nanosleep
	gensys  37, alarm
	gensys  57, fork
	gensys  60, exit
	gensys  79, getcwd
	gensys  80, chdir
	gensys  82, rename
	gensys  83, mkdir
	gensys  84, rmdir
	gensys  85, creat
	gensys  86, link
	gensys  88, unlink
	gensys  89, readlink
	gensys  90, chmod
	gensys  92, chown
	gensys  95, umask
	gensys  96, gettimeofday
	gensys 102, getuid
	gensys 104, getgid
	gensys 105, setuid
	gensys 106, setgid
	gensys 107, geteuid
	gensys 108, getegid
	gensys 127, rt_sigpending

	global open:function
open:
	call	sys_open
	cmp		rax, 0
	jge		open_success	; no error :)
open_error:
	neg		rax
%ifdef NASM
	mov		rdi, [rel errno wrt ..gotpc]
%else
	mov		rdi, [rel errno wrt ..gotpcrel]
%endif
	mov		[rdi], rax	; errno = -rax
	mov		rax, -1
	jmp		open_quit
open_success:
%ifdef NASM
	mov		rdi, [rel errno wrt ..gotpc]
%else
	mov		rdi, [rel errno wrt ..gotpcrel]
%endif
	mov		QWORD [rdi], 0	; errno = 0
open_quit:
	ret

    global __myrt:function
__myrt:
    mov     rax, 15
    syscall
    ret

	global setjmp:function
setjmp:
	pop		rsi ; get return address
	push	rsi
	
	mov 	QWORD [rdi + 8 * 0], rbx	; jmp_buf[0].reg[0] = rbx
	mov 	QWORD [rdi + 8 * 1], rsp	; jmp_buf[0].reg[1] = rsp
	mov 	QWORD [rdi + 8 * 2], rbp	; jmp_buf[0].reg[2] = rbp
	mov 	QWORD [rdi + 8 * 3], r12	; jmp_buf[0].reg[3] = r12
	mov 	QWORD [rdi + 8 * 4], r13	; jmp_buf[0].reg[4] = r13
	mov 	QWORD [rdi + 8 * 5], r14	; jmp_buf[0].reg[5] = r14
	mov 	QWORD [rdi + 8 * 6], r15	; jmp_buf[0].reg[6] = r15
	mov 	QWORD [rdi + 8 * 7], rsi	; jmp_buf[0].reg[7] = rsi

	mov		rdi, 2				; how = SIG_SETMASK
	mov		rsi, 0				; set = NULL
	lea		rdx, [rdi + 8 * 8]	; oldset = &jmp_buf[0].mask
	mov		rcx, 4				; sigsize = 4
	call	sys_rt_sigprocmask

	mov		rax, 0	; return 0
	ret

	global longjmp:function
longjmp:
	mov 	rbx, QWORD [rdi + 8 * 0]	; rbx = jmp_buf[0].reg[0]
	mov 	rsp, QWORD [rdi + 8 * 1]	; rsp = jmp_buf[0].reg[1]
	mov 	rbp, QWORD [rdi + 8 * 2]	; rbp = jmp_buf[0].reg[2]
	mov 	r12, QWORD [rdi + 8 * 3]	; r12 = jmp_buf[0].reg[3]
	mov 	r13, QWORD [rdi + 8 * 4]	; r13 = jmp_buf[0].reg[4]
	mov 	r14, QWORD [rdi + 8 * 5]	; r14 = jmp_buf[0].reg[5]
	mov 	r15, QWORD [rdi + 8 * 6]	; r15 = jmp_buf[0].reg[6]

	pop		rax
	mov 	rax, QWORD [rdi + 8 * 7]	; rsi = jmp_buf[0].reg[7]
	push	rax

	push	rsi					; val
	mov		rdi, 2				; how = SIG_SETMASK
	lea		rsi, [rdi + 8 * 8]	; set = &jmp_buf[0].mask
	mov		rdx, 0				; oldset = NULL
	mov		rcx, 4				; sigsize = 4
	call	sys_rt_sigprocmask

	pop		rax	; return val
	ret