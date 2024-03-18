/*
	PassMeLoader
	Copyright (C) 2024 lifehackerhansol

	SPDX-License-Identifier: 0BSD
*/

#include <stdio.h>
#include <nds.h>

#include "load_bin.h"

void swiSoftReset0(void);

#define VRAM_BANK_C_LCDC (u16*)0x06840000

int main(void) {
	consoleDemoInit();
	printf("Starting PassMe...");

	// Get access to LCDC bank
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_LCD;
	// and load the bootloader there
	memcpy (VRAM_BANK_C_LCDC, load_bin, load_bin_size);

	// Give ARM7... basically everything
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_ARM7_0x06000000;
	sysSetBusOwners(BUS_OWNER_ARM7, BUS_OWNER_ARM7);

	// enter PassMe loop
	*((vu32*)0x02FFFE04) = (u32)0xE59FF018;  // ldr pc, 0x02FFFE24
	__NDSHeader->arm9executeAddress = (void*)0x02FFFE04;  // Set ARM9 Loop address

	// ARM7, here we come for the bootloader!
	resetARM7(0x06000000);

	// Reset
	swiSoftReset0();

	return 0;
}
