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
	SMS_loadBGPalette(tiles_palette_bin);
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
	
	if (player.x < 0) {
		player.x = 0;
	} else if (player.x > 240) {
		player.x = 240;
	}
	
	if (player.y < PLAYER_TOP) {
		player.y = PLAYER_TOP;
	} else if (player.y > PLAYER_BOTTOM) {
		player.y = PLAYER_BOTTOM;
	}
	
	if (joy & (PORT_A_KEY_1 | PORT_A_KEY_2)) {
	}
}

void handle_base_movement() {
	move_actor(&base);

	if (base.x < -8 || base.x > 248 || base.y < (PLAYER_TOP - 8) || base.y > (PLAYER_BOTTOM + 8)) {
		init_actor(&base, 224, 88, 2, 1, 128, 6);
	}
	
	switch (base.state) {
	case 0:
		// Initialize
		base.state = 1;
		base.state_timer = 120;
		break;
		
	case 1:
		if (!base.state_timer) {
			// Morph into a projectile and wait a bit more
			init_actor(&base, base.x, base.y, 2, 1, 128 + 24, 6);
			base.state = 2;
			base.state_timer = 120;
		}
		break;
		
	case 2:
		// Aim at the player and attack
		aim_actor_towards(&base, &player);
		base.state = 3;
		break;
	}	
}

void handle_seeker_movement() {
	move_actor(&seeker);
	
	switch (seeker.state) {
	case 0:
		// Initialize
		seeker.state_timer = 0;
		seeker.state = 1;		
		break;
	
	case 1:
		if (!seeker.state_timer) {
			aim_actor_towards(&seeker, &player);
			seeker.state_timer = 30;
		}
		break;
	}
	
	if (seeker.x < 0) {
		seeker.x = 0;
	} else if (seeker.x > 240) {
		seeker.x = 240;
	}
	
	if (seeker.y < PLAYER_TOP) {
		seeker.y = PLAYER_TOP;
	} else if (seeker.y > PLAYER_BOTTOM) {
		seeker.y = PLAYER_BOTTOM;
	}
}

fill_background() {
	SMS_setNextTileatXY(0, 0);
	for (int i = (SCREEN_CHAR_W * SCREEN_CHAR_H); i; i--) {
		SMS_setTile(256 + (rand() & 0x03));
	}
}

draw_barrier() {
	static char x, y;
	
	for (y = 4; y < (SCREEN_CHAR_H - 4); y++) {
		x = 20;
		SMS_setNextTileatXY(x, y);
		for (; x < SCREEN_CHAR_W; x++) {
			SMS_setTile((256 + 4) + (rand() & 0x03));
		}
	}
}

void main() {
	SMS_useFirstHalfTilesforSprites(1);
	SMS_setSpriteMode(SPRITEMODE_TALL);
	SMS_VDPturnOnFeature(VDPFEATURE_HIDEFIRSTCOL);

	SMS_displayOff();
	SMS_loadPSGaidencompressedTiles(sprites_tiles_psgcompr, 0);
	SMS_loadPSGaidencompressedTiles(tiles_tiles_psgcompr, 256);
	load_standard_palettes();
	
	fill_background();
	draw_barrier();

	SMS_displayOn();

	init_actor(&player, 32, 15, 2, 1, 64, 16);
	init_actor(&base, 224, 88, 2, 1, 128, 6);
	init_actor(&seeker, 128, 64, 2, 1, 192, 5);	
	
	while (1) {
		handle_player_input();
		handle_base_movement();
		handle_seeker_movement();
		
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
SMS_EMBED_SDSC_HEADER(0,2, 2021,9,17, "Haroldo-OK\\2021", "Rassak's Retribution",
  "An Yar's Revenge clone.\n"
  "Built using devkitSMS & SMSlib - https://github.com/sverx/devkitSMS");
