// Itamar Laredo
  .section  .rodata
  .text
.globl  go
  .type   go, @function
go:
	xorl	%ecx, %ecx            # i = 0
	xorl	%eax, %eax	      # sum = 0
	jmp	.L2                   # jump to loop condition
.L3:
	movl	(%rdi, %rcx, 4), %esi # esi = 4*i + a[0] --> A[i]
	test	$1, %esi	      # esi & 1
	je 	.L4		      # even ZF=1
	addl    %esi, %eax            # sum += A[i]
	addl	$1, %ecx	      # i++
	jmp     .L2                   # jump to loop condition
.L4:
	movl	%esi, %edx	      # edx = A[i]
	sall	%cl, %edx	      # edx << i
	addl    %edx, %eax            # sum += A[i]
	addl    $1, %ecx              # i++
	jmp     .L2                   # jump to loop condition
.L2:
	cmpq	$10, %rcx             # i - 10 = ?
	jne	.L3		      # if i < 10 goto L3
	ret 		              # otherwise return
