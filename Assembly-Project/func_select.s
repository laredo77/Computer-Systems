# Itamar Laredo
    .section    .rodata
get_string:  .string "%s"
get_int:    .string "%d"
txt_error:  .string "invalid option!\n"
txt_opt5060:   .string "first pstring length: %d, second pstring length: %d\n"
txt_opt52:  .string "old char: %c, new char: %c, first string: %s, second string: %s\n"
txt_opt53:   .string "length: %d, string: %s\n"
txt_opt54:   .string "length: %d, string: %s\n"
txt_opt55:   .string "compare result: %d\n"

    .align 8 # align address to multiple of 8
    .L50:
        .quad   .L0   # case 50
        .quad   .L404 # Invalid option
        .quad   .L2   # case 52
        .quad   .L3   # case 53
        .quad   .L4   # case 54
        .quad   .L5   # case 55
        .quad   .L404 # Invalid option
        .quad   .L404 # Invalid option
        .quad   .L404 # Invalid option
        .quad   .L404 # Invalid option
        .quad   .L0   # case 60

    .text
.global	run_func
	.type	run_func, @function
run_func:
    # %rdi = Option, %rsi = p1, %rdx = p2
    leaq    -50(%rdi), %r12  # normalize option and save it in r12
    cmpq    $10, %r12        # compare option to 10
    ja      .L404            # if above, jump to defualt case (exit)
    jmp     *.L50(, %r12, 8) # Goto jt[Option]

.L0:
    movq    %rsi, %rdi  # rdi = p1
    xorq    %rax, %rax  # rax = 0
    call    pstrlen     # calling pstrlen function
    movsbq  %al, %rsi   # save the length in rsi

    movq    %rdx, %rdi  # rdi = p2
    xorq    %rax, %rax  # rax = 0
    call    pstrlen     # calling pstrlen function
    movsbq  %al, %rdx   # save the length in rdx

    movq    $txt_opt5060, %rdi # format string option 1 in rdi
    xorq    %rax, %rax         # rax = 0
    call    printf             # printing (rdi, rsi, rdx)
    ret
.L2:
    # %rdi = Option, %rsi = p1, %rdx = p2
    pushq   %rbp
    movq    %rsp, %rbp
    addq    $-16, %rsp
    pushq   %r12
    movq    %rsi, %r12 # r12 = p1
    pushq   %r13
    movq    %rdx, %r13 # r13 = p2
    pushq   %r14       # for old char
    pushq   %r15       # for new char

    #old char
    movq    $get_string, %rdi
    leaq    8(%rsp), %rsi
    xorq    %rax, %rax
    call    scanf         # old char save in 8(%rsp)
    movzbq  8(%rsp), %r14 # r14 = old char

    # new char
    movq    $get_string, %rdi # get second char
    leaq    (%rsp), %rsi
    xorq    %rax, %rax
    call    scanf             # new char saved in (%rsp)
    movzbq  (%rsp), %r15      # r15 = old char

    # replace p1
    movq    %r12, %rdi # p1
    movq    %r14, %rsi # old char
    movq    %r15, %rdx # new char
    call    replaceChar
    movq    %rax, %r12 # r12 = result

    # replace p2
    movq    %r13, %rdi  # p2
    movq    %r14, %rsi  # old char
    movq    %r15, %rdx  # new char
    call    replaceChar
    movq    %rax, %r13  # r13 = result

    # printing total result
    movq    $txt_opt52, %rdi
    movq    %r14, %rsi
    movq    %r15, %rdx
    addq    $1, %r12
    movq    %r12, %rcx
    addq    $1, %r13
    movq    %r13, %r8
    xorq    %rax, %rax
    call    printf

    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    addq    $16, %rsp
    movq    %rbp, %rsp
    popq    %rbp
    ret
.L3:
    pushq   %rbp
    movq    %rsp, %rbp
    addq    $-16, %rsp # place for 2 int's
    pushq   %r12
    movq    %rsi, %r12 # r12 = p1
    pushq   %r13
    movq    %rdx, %r13 # r13 = p2
    pushq   %r14       # start index
    pushq   %r15       # end index

    #get start index
    leaq    4(%rsp), %rsi   # 4 bytes for i
    movq    $get_int, %rdi
    xorq    %rax, %rax
    call    scanf
    movsbq   4(%rsp), %r14 # r14 = start index

    # get end index
    leaq    (%rsp), %rsi   # 4 bytes for j
    movq    $get_int, %rdi # get second char
    xorq    %rax, %rax     # rax = 0
    call    scanf
    movsbq  (%rsp), %r15   # r15 = end index

    # j = end index, i = start index, src = p2, dst = p1
    movq    %r12, %rdi  # first arg: p1 (dst)
    movq    %r13, %rsi  # second arg: p2 (src)
    movq    %r14, %rdx  # third arg: start index (i)
    movq    %r15, %rcx  # forth arg: end index (j)
    xorq    %rax, %rax
    call    pstrijcpy
    movq    %rax, %r12  # r12 = result

    #printing dst
    movq    $txt_opt53, %rdi
    movzbq  (%r12), %rsi
    addq    $1, %r12
    movq    %r12, %rdx
    xorq    %rax, %rax
    call    printf

    #printing src
    movq    $txt_opt53, %rdi
    movzbq  (%r13), %rsi # length
    incq    %r13         # src++
    movq    %r13, %rdx   # rdx = src
    xorq    %rax, %rax
    call    printf

    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    addq    $16, %rsp
    movq    %rbp, %rsp
    popq    %rbp
    ret
.L4:
    pushq   %rbp
    movq    %rsp, %rbp
    pushq   %r15
    movq    %rdx, %r15 # r15 = p2

    movq    %rsi, %rdi # rdi = p1
    xorq    %rax, %rax
    call    swapCase
    movq    %rax, %r12 # r12 = swapper p1

    # printing pstr1
    movq    $txt_opt54, %rdi
    movzbq  (%r12), %rsi
    addq    $1, %r12
    movq    %r12, %rdx
    xorq    %rax, %rax
    call    printf

    movq    %r15, %rdi  # rdi = p2
    xorq    %rax, %rax
    call    swapCase
    movq    %rax, %r13  # r13 = swapper p2

    # printing pstr2
    movq    $txt_opt54, %rdi
    movzbq  (%r13), %rsi
    addq    $1, %r13
    movq    %r13, %rdx
    xorq    %rax, %rax
    call    printf

    popq    %r15
    movq    %rbp, %rsp
    popq    %rbp
	ret
.L5:
    pushq   %rbp
    movq    %rsp, %rbp
    addq    $-16, %rsp # place for 2 int's
    pushq   %r12
    movq    %rsi, %r12 # r12 = p1
    pushq   %r13
    movq    %rdx, %r13 # r13 = p2
    pushq   %r14       # start index
    pushq   %r15       # end index

    #get start index
    leaq    4(%rsp), %rsi  # 4 bytes for i
    movq    $get_int, %rdi
    xorq    %rax, %rax
    call    scanf
    movsbq   4(%rsp), %r14 # r14 = start index

    # get end index
    leaq    (%rsp), %rsi   # 4 bytes for j
    movq    $get_int, %rdi # get second char
    xorq    %rax, %rax     # rax = 0
    call    scanf
    movsbq  (%rsp), %r15   # r15 = end index

    movq    %r12, %rdi  # first arg: p1
    movq    %r13, %rsi  # second arg: p2
    movq    %r14, %rdx  # third arg: i
    movq    %r15, %rcx  # forth arg: j
    xorq    %rax, %rax
    call    pstrijcmp
    movq    %rax, %rsi  # save the result

    # printing
    movq    $txt_opt55, %rdi
    xorq    %rax, %rax
    call    printf

    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    addq    $16, %rsp
    movq    %rbp, %rsp
    popq    %rbp
	ret
.L404:
    movq    $txt_error, %rdi # format string error to rdi
    xorq    %rax, %rax       # rax = 0
    call    printf           # printing message
    ret
