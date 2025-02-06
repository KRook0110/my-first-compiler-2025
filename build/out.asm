section .text
	global _start
_start:
	call main
	mov rdx, rax
	mov rax, 60
	syscall
main:
	push rbp
	mov rbp, rsp
	mov rax, 7
	push rax
	mov rax, 6
	pop rbx
	imul rax, rbx
	push rax
	mov rax, 5
	pop rbx
	add rax, rbx
	push rax
	mov rax, 4
	push rax
	mov rax, 3
	pop rbx
	imul rax, rbx
	pop rbx
	cqo
	idiv rbx
	push rax
	mov rax, 2
	push rax
	mov rax, 1
	pop rbx
	add rax, rbx
	pop rbx
	add rax, rbx
	pop rbp
	ret
	pop rbp
	ret
