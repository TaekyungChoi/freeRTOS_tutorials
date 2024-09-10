@ Random number generator
@
@ This uses a 33-bit feedback shift register to generate a pseudo-randomly
@ ordered sequence of numbers which repeats in a cycle of length 2^33 - 1
@ NOTE: randomseed should not be set to 0, otherwise a zero will be generated
@ continuously (not particularly random!).
@
@ This is a good application of direct ARM assembler, because the 33-bit
@ shift register can be implemented using RRX (which uses reg + carry).
@ An ANSI C version would be less efficient as the compiler would not use RRX.


    @AREA  |C$$code|, CODE, READONLY
@|C$$code|:
    .section .text,"x"
  	.thumb
	.syntax unified
	.global  randomnumber
randomnumber:
@ on exit:
@	a1 = low 32-bits of pseudo-random number
@	a2 = high bit (if you want to know it)
	ldr     ip, =seedpointer
	ldmia   ip, {a1, a2}
	tst     a2, a2, LSR #1           @ to bit into carry
	movs    a3, a1, RRX             @ 33-bit rotate right
	adc     a2, a2, a2              @ carry into LSB of a2
	eor     a3, a3, a1, LSL#12      @ (involved!)
	eor     a1, a3, a3, LSR#20      @ (similarly involved!)
	stmia   ip, {a1, a2}

	bx lr

@*****************************************************************************
@
@ seedrand set seed number
@
@*****************************************************************************
	.global  seedrand
seedrand:
    @
    @ Save the non-volatile registers which we will use.
    @
    push {r1}

	@
	@ Get a pointer to the state variables.
	@
	ldr     r1, =seed
	str     r0, [r1, #0]

	pop {r1}

    @
    @ Return to the caller.
    @
    bx lr

		.section .data,"a"
        .global  seed
seedpointer:
        .word     seed
seed:
        .word     0xAAAAAAAA
        .word     0x55555555
        .word     0xBBBBBBBB

        .end
