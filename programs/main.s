bits 64
section .bss
  tape resb 30000

section .text
global _start
_start:
  lea rbx, [rel tape]
  add byte [rbx], 10
.L0_beg:
  cmp byte [rbx], 0
  je .L0_end
  add rbx, 1
  add byte [rbx], 7
  add rbx, 1
  add byte [rbx], 10
  add rbx, 1
  add byte [rbx], 3
  add rbx, 1
  add byte [rbx], 1
  add rbx, -4
  add byte [rbx], -1
  cmp byte [rbx], 0
  jne .L0_beg
.L0_end:
  add rbx, 1
  add byte [rbx], 2
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add rbx, 1
  add byte [rbx], 1
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add byte [rbx], 7
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add byte [rbx], 3
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add rbx, 1
  add byte [rbx], 2
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add rbx, -2
  add byte [rbx], 15
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add rbx, 1
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add byte [rbx], 3
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add byte [rbx], -6
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add byte [rbx], -8
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add rbx, 1
  add byte [rbx], 1
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  add rbx, 1
  mov rax, 1
  mov rdi, 1
  mov rsi, rbx
  mov rdx, 1
  syscall
  mov rax, 60
  xor rdi, rdi
  syscall
