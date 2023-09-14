/*
	PassMeLoader
	Copyright (C) 2010-2016 Taiju Yamada
	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: BSD-2-Clause
*/

#include <stdio.h>
#include <nds.h>

static void resetMemory2_ARM9(void) {
	register int i;

	//clear out ARM9 DMA channels
	for (i = 0; i < 4; i++) {
		DMA_CR(i) = 0;
		DMA_SRC(i) = 0;
		DMA_DEST(i) = 0;
		TIMER_CR(i) = 0;
		TIMER_DATA(i) = 0;
	}

	VRAM_CR = 0x80808080;
	(*(vu32*)0x02FFFE04) = 0;   // temporary variable
	BG_PALETTE[0] = 0xFFFF;
	dmaFillWords(0x02FFFE04, BG_PALETTE+1, (2*1024)-2);
	dmaFillWords(0x02FFFE04, OAM,     2*1024);
	dmaFillWords(0x02FFFE04, (void*)0x04000000, 0x56);  //clear main display registers
	dmaFillWords(0x02FFFE04, (void*)0x04001000, 0x56);  //clear sub  display registers
	dmaFillWords(0x02FFFE04, VRAM,  656*1024);

	REG_DISPSTAT=0;
	videoSetMode(0);
	videoSetModeSub(0);
	VRAM_A_CR = 0;
	VRAM_B_CR = 0;
	VRAM_C_CR = 0;
	VRAM_D_CR = 0;
	VRAM_E_CR = 0;
	VRAM_F_CR = 0;
	VRAM_G_CR = 0;
	VRAM_H_CR = 0;
	VRAM_I_CR = 0;
	VRAM_CR   = 0x00000000;
	REG_POWERCNT = 0x820F;
	//REG_EXMEMCNT = 0xe880;

	//set shared ram to ARM7
	WRAM_CR = 0x03;
}

void swiSoftReset0(void);

void bootMoonlight(void){
	printf("resetMemory2_ARM9\n");
	resetMemory2_ARM9();

	fifoSendValue32(FIFO_USER_01, 1);
	REG_IME = 0;
	REG_IE  = 0;
	REG_IF  = ~0;

	printf("Passme reset...");
	IC_InvalidateAll();
	DC_FlushAll();
	DC_InvalidateAll();

	// enter PassMe loop
	*((vu32*)0x02FFFE04) = (u32)0xE59FF018;  // ldr pc, 0x02FFFE24
	__NDSHeader->arm9executeAddress = (void*)0x02FFFE04;  // Set ARM9 Loop address
	swiSoftReset0();  // Reset
	printf("Failed.\n");
	while(1);
}

void boot_passme(){
	sysSetBusOwners(BUS_OWNER_ARM9, BUS_OWNER_ARM9);
	sysSetCartOwner(BUS_OWNER_ARM7);
	bootMoonlight();
	while(1);
}

int main(void) {

	consoleDemoInit();

	printf("Starting PassMe...");
	boot_passme();

	return 0;
}
