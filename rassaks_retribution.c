#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib/SMSlib.h"
#include "lib/PSGlib.h"
#include "data.h"

void load_standard_palettes() {
	SMS_loadBGPalette(sprites_palette_bin);
	SMS_loadSpritePalette(sprites_palette_bin);
}

void main() {
	SMS_useFirstHalfTilesforSprites(1);
	SMS_setSpriteMode(SPRITEMODE_TALL);
	SMS_VDPturnOnFeature(VDPFEATURE_HIDEFIRSTCOL);

	SMS_displayOff();
	SMS_loadPSGaidencompressedTiles(sprites_tiles_psgcompr, 0);
	load_standard_palettes();

	SMS_displayOn();
	
	while (1) {
	}
}

SMS_EMBED_SEGA_ROM_HEADER(9999,0); // code 9999 hopefully free, here this means 'homebrew'
SMS_EMBED_SDSC_HEADER(0,1, 2021,9,13, "Haroldo-OK\\2021", "Rassak's Retribution",
  "An Yar's Revenge clone.\n"
  "Built using devkitSMS & SMSlib - https://github.com/sverx/devkitSMS");
