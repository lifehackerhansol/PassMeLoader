#include <nds/asminc.h>

    .arm

BEGIN_ASM_FUNC SwitchUserMode
	push {r0}
	mov r0, #0x1F
	msr cpsr, r0
	pop {r0}
	bx lr
