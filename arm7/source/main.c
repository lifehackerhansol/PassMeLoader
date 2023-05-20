/*
	PassMeLoader
	Copyright (C) 2010-2016 Taiju Yamada
	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: BSD-2-Clause
*/

#include <nds.h>

void VblankHandler(void) {
}

void SwitchUserMode();

void resetMoonlight(void) {
	u32 i;
	REG_IME = IME_DISABLE;	// Disable interrupts
	REG_IF = REG_IF;	// Acknowledge interrupt

	for(i=0x04000400; i<0x04000500; i+=4)
		*((u32*)i)=0;

	REG_SOUNDCNT = 0;
	//clear out ARM7 DMA channels and timers
	for(i=0x040000B0;i<(0x040000B0+0x30);i+=4)
		*((vu32*)i)=0;
	for(i=0x04000100;i<0x04000110;i+=2)
		*((u16*)i)=0;
	SwitchUserMode();

	REG_IE = 0;
	REG_IF = ~0;
	(*(vu32*)(0x04000000-4)) = 0;  //IRQ_HANDLER ARM7 version
	(*(vu32*)(0x04000000-8)) = ~0; //VBLANK_INTR_WAIT_FLAGS, ARM7 version
	REG_POWERCNT = 1;  //turn off power to stuffs

	while(__NDSHeader->arm9executeAddress != (void*)0x02FFFE04 && __NDSHeader->arm9executeAddress != (void*)0x0CFFFE04);
	__NDSHeader->arm7executeAddress = (void*)0x080000C0;	// Bootloader start address
	swiSoftReset();
	while(1);
}

int main(void) {
	ledBlink(0);

	irqInit();
	fifoInit();
	installSystemFIFO();
	irqSet(IRQ_VBLANK, VblankHandler);
	irqEnable(IRQ_VBLANK);

	// Keep the ARM7 out of main RAM
	while(1) {
		swiWaitForVBlank();

		if(fifoCheckValue32(FIFO_USER_01)) {
			fifoGetValue32(FIFO_USER_01);
			resetMoonlight();
		}
	}
	return 0;
}
