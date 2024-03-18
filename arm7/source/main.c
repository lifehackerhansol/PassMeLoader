/*
	PassMeLoader
	Copyright (C) 2010-2016 Taiju Yamada
	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: BSD-2-Clause
*/

#include <nds.h>

void swiSoftReset0(void);

void VblankHandler(void) {
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
			swiSoftReset0();
		}
	}

	// Should never reach this point
	while(1) swiWaitForVBlank();

	return 0;
}
