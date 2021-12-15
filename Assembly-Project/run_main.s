# Itamar Laredo
    .section    .rodata
get_string:  .string "%s"
get_int:    .string "%d"
    .text
.global	run_main
	.type	run_main, @function
run_main:
    pushq   %rbp
    movq    %rsp, %rbp
    subq    $528, %rsp # 2 * ( 4B for len(p) + 256B for p) + 4B for option + 4B for alignment

    #Address: rbp=0, rsp=-528

    # len p1
    movq    $get_int, %rdi
    leaq    8(%rsp), %rsi   # address of len(p1)
    xorq    %rax, %rax
    call    scanf

    # p1
    movq    $get_string, %rdi
    leaq    17(%rsp), %rsi  # addr++, and address of p1
    xorq    %rax, %rax
    call    scanf
    movq    8(%rsp), %rsi   # rsi = len(p1)
    movb    %sil, 16(%rsp)  # push p1 to the stack

    # len p2
    movq    $get_int, %rdi
    leaq    8(%rsp), %rsi   # address of len(p2)
    xorq    %rax, %rax
    call    scanf

    # p2
    movq    $get_string, %rdi
    leaq    273(%rsp), %rsi # addr++, and address of p1
    xorq    %rax, %rax
    call    scanf
    movq    8(%rsp), %rsi   # rsi = len(p2)
    movb    %sil, 272(%rsp) # push p2 to the stack

    #option
    movq    $get_int, %rdi
    leaq    520(%rsp), %rsi # address of option
    xorq    %rax, %rax
    call    scanf

    movq    520(%rsp), %rdi # option
    leaq    16(%rsp), %rsi  # p1
    leaq    272(%rsp), %rdx # p2
    call    run_func        # call run_func with arguments

    xorq    %rax, %rax
    addq    $528, %rsp
    movq    %rbp, %rsp
    popq    %rbp
    ret
