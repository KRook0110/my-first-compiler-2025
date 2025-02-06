section .text
	global _start
_start:
	call main
	mov rdi, rax
	mov rax, 60
	syscall
main:
	push rbp
	mov rbp, rsp
	mov rax, 7
	push rax
	mov rax, 6
	pop rbx
	sub rax ,rbx
	push rax
	mov rax, 5
	push rax
	mov rax, 4
	pop rbx
	imul rax, rbx
	pop rbx
	cqo
	idiv rbx
	mov rax, rdx
	push rax
	mov rax, 3
	push rax
	mov rax, 2
	push rax
	mov rax, 1
	pop rbx
	cqo
	idiv rbx
	pop rbx
	imul rax, rbx
	pop rbx
	add rax, rbx
	pop rbp
	ret
	pop rbp
	ret
