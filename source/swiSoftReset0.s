#include <nds/asminc.h>

    .arm

@ void swiSoftReset0(void)
BEGIN_ASM_FUNC swiSoftReset0
    swi 0
