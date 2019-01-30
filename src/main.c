#include <gb/gb.h>
#include <stdio.h>
#include <rand.h>
#include "structs/ship.c"
#include "structs/bullet.c"
#include "sprites/bkgSprites.c"
#include "sprites/sprites.c"
#include "tiles/bkgFloor.c"

// declaration

void init();
void createShip();
void checkInput();
void moveShip();
void moveBullets();
void moveBkg();
//void enemyMove();
//void enemyCreate();
//void enemyDestroy();
void updateSwitches();

UINT8 limRand(UINT8, UINT8);
UINT8 collisionCheck(UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8);

UINT8 x,y,i,j,timer,visible,enemy[2],enemyDir[2];

ship_struct ship;

void main() {
	init();

	while(1) {
		checkInput();     // Check for user input (and act on it)
		updateSwitches(); // Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		moveBkg();
        wait_vbl_done();  // Wait until VBLANK to avoid corrupting visual memory
		delay(10);
	}
}

void init() {
	DISPLAY_ON; // Turn on the display

	set_bkg_data(0,24,bkgSrpites);
	for(y = 0; y < 9; y++) {
		set_bkg_tiles(0,((y*4)-4)+i,20,4,bkgFloor);
	}
    
	// Load the 'sprites' tiles into sprite memory
	set_sprite_data(0, 22, sprites);
	
	i = 0;
	timer = 0;
	visible = 0;
	/*enemyDir[0] = 0;
	enemyDir[1] = 0;
	enemy[0] = 0;
	enemy[1] = 0;*/

	createShip();
}

void createShip() {

	ship.pos_x			= 80;
	ship.pos_y			= 124;
	ship.ship_tiles		= 4;
	ship.iter_tile		= 0;
	ship.shoot_status	= 0;
	ship.bullet_tile	= 4;
	ship.max_bullets	= 5;
	ship.bullet_timer	= 0;
	ship.bullet_trigger = 100;
	
	// Set the first movable sprite (0) to be the first tile in the sprite memory (0)
	for(ship.iter_tile; ship.iter_tile < ship.ship_tiles; ship.iter_tile++) {
		set_sprite_tile(ship.iter_tile,ship.iter_tile);
	}

	moveShip();
}

void moveShip() {
	UINT8 ship_x,ship_y;

	for(ship.iter_tile = 0; ship.iter_tile < ship.ship_tiles; ship.iter_tile++) {
		ship_x = ship.pos_x;
		ship_y = ship.pos_y;

		if(ship.iter_tile == 1 || ship.iter_tile == 3) {
			ship_y = ship.pos_y + 8;
		}

		if(ship.iter_tile == 2 || ship.iter_tile == 3) {
			ship_x = ship.pos_x + 8;
		}

		move_sprite(ship.iter_tile,ship_x,ship_y);
	}
}

void moveBullets() {
	
	/*if(ship.shoot_status == 1) {
		ship.shoot_status = 2;
		set_sprite_tile(ship.bullet_tile,13);
		bullet.pos_x = ship.pos_x + 4;
		bullet.pos_y = ship.pos_y - 8;
	}
	if(bullet.pos_y > 0 && bullet.pos_y < 200 && ship.shoot_status == 2) {
		move_sprite(bullet.bullet_tile,bullet.pos_x,bullet.pos_y);
		bullet.pos_y -= 3;
	} else {
		ship.shoot_status = 0;
	}*/
}

void moveBkg() {
	scroll_bkg(0,-1); 
}

void updateSwitches() {
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
}

/*
void enemyCreate() {
	if(visible == 0) { timer++; }
	if(timer > 254 && visible == 0) {
		timer = 0;
		visible = 1;
		enemy[0] = limRand(24,144);
		enemy[1] = limRand(24,144);
		set_sprite_tile(1,0);
	}
}

void enemyMove() {
	if(visible == 1) {
		if(enemy[0] > 143) { enemyDir[0] = 1; }
		if(enemy[0] < 24)  { enemyDir[0] = 0; }
		if(enemy[1] > 135) { enemyDir[1] = 1; }
		if(enemy[1] < 32)  { enemyDir[1] = 0; }

		if(enemyDir[0] == 0) {
			enemy[0]++;
		} else {
			enemy[0]--;
		}

		if(enemyDir[1] == 0) {
			enemy[1]++;
		} else {
			enemy[1]--;
		}

		move_sprite(1, enemy[0], enemy[1]);

		if(collisionCheck(player[0], player[1], 8, 8, enemy[0], enemy[1], 8, 8) == 1) {
			enemyDestroy();
		}
	}
}

void enemyDestroy() {
	visible = 0;
	for(i = 1; i < 6; i++) {
		delay(100);
		set_sprite_tile(1,i);
	}
	i = 0;
}
//*/

void checkInput() {

	j = joypad();
	if (j & J_A)     						{ ship.shoot_status = 1; } else { ship.shoot_status = 0; } // A = shoot
	if ((j & J_UP) && ship.pos_y > 24)		{ ship.pos_y--; } // UP
	if ((j & J_RIGHT) && ship.pos_x < 144)	{ ship.pos_x++; } // RIGHT
	if ((j & J_DOWN) && ship.pos_y < 136)	{ ship.pos_y++; } // DOWN
	if ((j & J_LEFT) && ship.pos_x > 18)	{ ship.pos_x--; } // LEFT

	moveBullets();
	moveShip();
}

/*
UINT8 limRand(UINT8 min, UINT8 max) {
	return rand() % max + min;
}

// Check if two rectangles from x1,y1, and extending out h1, h2, 
// overlap with another, x2,y2, and extending out w2, h2
UINT8 collisionCheck(UINT8 x1, UINT8 y1, UINT8 w1, UINT8 h1, UINT8 x2, UINT8 y2, UINT8 w2, UINT8 h2) {
	if ((x1 < (x2+w2)) && ((x1+w1) > x2) && (y1 < (h2+y2)) && ((y1+h1) > y2)) {
		return 1;
	} else {
		return 0;
	}
}*/
