#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib/SMSlib.h"
#include "lib/PSGlib.h"
#include "actor.h"
#include "data.h"

#define PLAYER_SPEED ((int) 1.5 * 256)
#define PLAYER_SHOT_SPEED (6)
#define PLAYER_TOP (16)
#define PLAYER_LEFT (8)
#define PLAYER_BOTTOM (SCREEN_H - 16)

actor player;
actor base;
actor seeker;

void load_standard_palettes() {
	SMS_loadBGPalette(sprites_palette_bin);
	SMS_loadSpritePalette(sprites_palette_bin);
}

void shuffle_random(char times) {
	for (; times; times--) {
		rand();
	}
}

void handle_player_input() {
	unsigned char joy = SMS_getKeysStatus();
	
	if (joy & PORT_A_KEY_UP) {
		player.incr_y.w -= PLAYER_SPEED;
		shuffle_random(1);
	} else if (joy & PORT_A_KEY_DOWN) {
		player.incr_y.w += PLAYER_SPEED;
		shuffle_random(2);
	}
	
	if (joy & PORT_A_KEY_LEFT) {
		player.incr_x.w -= PLAYER_SPEED;
		shuffle_random(3);
	} else if (joy & PORT_A_KEY_RIGHT) {
		player.incr_x.w += PLAYER_SPEED;
		shuffle_random(4);
	}
	
	move_actor(&player);
	if (player.y < PLAYER_TOP) {
		player.y = PLAYER_TOP;
	} else if (player.y > PLAYER_BOTTOM) {
		player.y = PLAYER_BOTTOM;
	}
	
	if (joy & (PORT_A_KEY_1 | PORT_A_KEY_2)) {
	}
}

void main() {
	SMS_useFirstHalfTilesforSprites(1);
	SMS_setSpriteMode(SPRITEMODE_TALL);
	SMS_VDPturnOnFeature(VDPFEATURE_HIDEFIRSTCOL);

	SMS_displayOff();
	SMS_loadPSGaidencompressedTiles(sprites_tiles_psgcompr, 0);
	load_standard_palettes();

	SMS_displayOn();

	init_actor(&player, 32, 15, 2, 1, 64, 16);
	init_actor(&base, 224, 88, 2, 1, 128, 6);
	init_actor(&seeker, 128, 64, 2, 1, 192, 5);
	
	while (1) {
		handle_player_input();
		
		SMS_initSprites();	
		draw_actor(&player);
		draw_actor(&base);
		draw_actor(&seeker);
		SMS_finalizeSprites();
		SMS_waitForVBlank();
		SMS_copySpritestoSAT();
	}
}

SMS_EMBED_SEGA_ROM_HEADER(9999,0); // code 9999 hopefully free, here this means 'homebrew'
SMS_EMBED_SDSC_HEADER(0,1, 2021,9,13, "Haroldo-OK\\2021", "Rassak's Retribution",
  "An Yar's Revenge clone.\n"
  "Built using devkitSMS & SMSlib - https://github.com/sverx/devkitSMS");
