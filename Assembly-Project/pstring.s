# Itamar Laredo
    .data
    .section .rodata
txt_string: .string "%s\n"
txt_invalid: .string "invalid input!\n"

    .text
.global pstrlen
    .type pstrlen, @function
pstrlen:
    movzbq  (%rdi), %rax
    ret

.global replaceChar
    .type replaceChar, @function
replaceChar:
    # rdi: pstr, rsi: oldchar, rdx: newchar
    pushq   %rbp
    movq    %rsp, %rbp
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15

    movq    %rdi, %r12   # r12 = pstr
    xorq    %r13, %r13   # i = 0
    movzbq  (%r12), %r14 # r14 = length(pstr)
    incq    %r12         # pstr++
    jmp     .replaceCharLoop

.replaceCharLoop:
    movzbq    (%r12), %r15 # r15 = current char
    cmp     %r15, %rsi     # current char == old char?
    je      .replaceCharSwap
    incq    %r12           # pstr++
    addq    $1, %r13       # i++
    cmp     %r13, %r14     # compare i to length(pstr)
    je      .replaceCharFinish
    jmp     .replaceCharLoop

.replaceCharSwap:
    movb    %dl, (%r12)    # pstr[i] = newchar
    incq    %r12           # pstr++
    addq    $1, %r13       # i++
    cmp     %r13, %r14     # compare i to length(pstr)
    je      .replaceCharFinish
    jmp     .replaceCharLoop

.replaceCharFinish:
    subq    %r14, %r12     # bring back the pointer of pstr to start address
    subq    $1, %r12
    movq    %r12, %rax

    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    movq    %rbp, %rsp
    popq    %rbp
    ret

.global pstrijcpy
    .type pstrijcpy, @function
pstrijcpy:
# rdi = dst, %rsi = src, %rdx = i, %rcx = j
    pushq   %rbp
    movq    %rsp, %rbp
    pushq   %r12
    movq    %rdi, %r12

    movzbq  (%rdi), %r10    # r10 = length(dst)
    cmpq     %rcx, %r10     # if j => length(dst)
    jle     .pstrijcpyInvalid
    movzbq  (%rsi), %r10    # r10 = length(src)
    cmpq     %rcx, %r10     # if j => length(src)
    jle     .pstrijcpyInvalid
    movzbq  (%rdi), %r10    # r10 = length(dst)
    cmpq     %rdx, %r10     # if i > length(dst)
    jl      .pstrijcpyInvalid
    movzbq  (%rsi), %r10    # r10 = length(src)
    cmpq     %rdx, %r10     # if i > length(src)
    jl      .pstrijcpyInvalid
    cmp     %rdx, %rcx      # if i > j
    jl      .pstrijcpyDone

    incq    %rsi       # src++
    incq    %rdi       # dst++
    addq    %rdx, %rsi # src[i]
    addq    %rdx, %rdi # dst[i]
    jmp     .pstrijcpyLoop

.pstrijcpyLoop:
    cmp     %rdx, %rcx
    je      .pstrijcpyFinish
    movzbq  (%rsi), %rax # rax = src[i]
    movb    %al, (%rdi)  # dst[i] = current char
    incq    %rdi         # dst++
    incq    %rsi         # src++
    incq    %rdx         # i++
    jmp     .pstrijcpyLoop

.pstrijcpyFinish:
    movzbq  (%rsi), %rax # rax = src[i]
    movb    %al, (%rdi)  # dst[i] = current char
    subq    %rcx, %rdi   # move back the pointer to start of dst
    decq    %rdi
    movq    %rdi, %rax   # return dst
    popq    %r12
    movq    %rbp, %rsp
    popq    %rbp
    ret

.pstrijcpyInvalid:
    movq    $txt_invalid, %rdi
    xorq    %rax, %rax
    call    printf
    movq    %r12, %rax # return dst
    popq    %r12
    movq    %rbp, %rsp
    popq    %rbp
    ret

.pstrijcpyDone:
    movq    %rdi, %rax # return dst
    popq    %r12
    movq    %rbp, %rsp
    popq    %rbp
    ret

.global swapCase
    .type swapCase, @function
swapCase:
#rdi = pstr
    movzbq  (%rdi), %r12 # r12 = length(pstr)
    incq    %rdi         # pstr++
    xorq    %r13, %r13   # i = 0
    jmp     .swapCaseLoop

.swapCaseLoop:
    cmpq    %r13, %r12     # len - i
    jle      .swapCaseDone # if i > len goto done
    movzbq  (%rdi), %rax   # rax = pstr[i]
    cmpq    $97, %rax
    jge     .swapCaseGreaterThan97 # rax hold value greater than 97 ('a')
    cmpq    $90, %rax
    jle     .swapCaseSmallerThan90 # rax hold value smaller than 90 ('Z')
    addq    $1, %r13 # i++
    incq    %rdi     # pstr++
    jmp     .swapCaseLoop

.swapCaseGreaterThan97:
    cmpq    $122, %rax
    jl      .swapCaseToUpper
    addq    $1, %r13 # i++
    incq    %rdi     # pstr++
    jmp     .swapCaseLoop

.swapCaseSmallerThan90:
    cmpq    $65, %rax
    jge     .swapCaseToLower
    addq    $1, %r13 # i++
    incq    %rdi     # pstr++
    jmp     .swapCaseLoop

.swapCaseToUpper:
    subq    $32, %rax
    movb    %al, (%rdi) # pstr[i] = currentchar -32
    addq    $1, %r13    # i++
    incq    %rdi        # pstr++
    jmp     .swapCaseLoop

.swapCaseToLower:
    addq    $32, %rax
    movb    %al, (%rdi) # pstr[i] = currentchar + 32
    addq    $1, %r13    # i++
    incq    %rdi        # pstr++
    jmp     .swapCaseLoop

.swapCaseDone:
    subq    %r12, %rdi
    decq    %rdi
    movq    %rdi, %rax
    ret

.global pstrijcmp
    .type pstrijcmp, @function
pstrijcmp:
# rdi:p1, rsi:p2 rdx:i rcx:j
    movzbq  (%rdi), %r10 # r10 = length(p1)
    cmpq     %rcx, %r10  # if j > length(p1)
    jl      .pstrijcmpInvalid
    movzbq  (%rsi), %r10 # r10 = length(p2)
    cmpq     %rcx, %r10  # if j > length(p2)
    jl      .pstrijcmpInvalid
    movzbq  (%rdi), %r10 # r10 = length(p1)
    cmpq     %rdx, %r10  # if i > length(p1)
    jl      .pstrijcmpInvalid
    movzbq  (%rsi), %r10 # r10 = length(p2)
    cmpq     %rdx, %r10  # if i > length(p2)
    jl      .pstrijcmpInvalid

    movq    %rdx, %r12
    addq    $1, %rdi
    addq    $1, %rsi
    addq    %r12, %rdi
    addq    %r12, %rsi
    jmp     .pstrijcmpLoop

.pstrijcmpLoop:
    cmp     %r12, %rcx    # j - i
    jl      .pstrijcmpFinish
    movzbq  (%rdi), %rax  # rax = p1[i]
    movzbq  (%rsi), %rdx  # rdx = p2[i]
    cmp     %al, %dl      # cmp p1[i] to p2[i]
    jl      .pstrijcmpOne # pstr1 is greater
    jg      .pstrijcmpTwo # pstr2 is greater
    incq    %rdi    # pstr1++
    incq    %rsi    # pstr2++
    addq    $1, %r12
    jmp     .pstrijcmpLoop

.pstrijcmpOne:
    movq    $1, %rax
    ret

.pstrijcmpTwo:
    movq    $-1, %rax
    ret

.pstrijcmpFinish:
    movq    $0, %rax
    ret

.pstrijcmpInvalid:
    movq    $txt_invalid, %rdi
    xorq    %rax, %rax
    call    printf
    movq    $-2, %rax
    ret
