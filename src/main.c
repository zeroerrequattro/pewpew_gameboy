#include <gb/gb.h>
#include <rand.h>
#include "structs/ship.c"
#include "structs/bullet.c"
#include "sprites/bkgSprites.c"
#include "sprites/sprites.c"
#include "tiles/bkgFloor.c"

// declaration

void init();
void createShip();
void createBullets();
void checkInput();
void checkDirection();
void moveShip();
void checkShoot();
void moveBullets();
void moveBkg();
void updateSwitches();

UINT8 limRand(UINT8, UINT8);
UINT8 collisionCheck(UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8);

UINT8 x,y,i,j,timer,visible;

ship_struct ship;
bullet_struct bullets[5];

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

	set_bkg_data(0,24,bkgSrpites);
	for(y = 0; y < 9; y++) {
		set_bkg_tiles(0,((y*4)-4)+i,20,4,bkgFloor);
	}
    
	// Load the 'sprites' tiles into sprite memory
	set_sprite_data(0, 22, sprites);
	
	i = 0;
	timer = 0;
	visible = 0;

	createShip();
	createBullets();

	DISPLAY_ON; // Turn on the display
}

void createShip() {
	ship.pos_x			= 80;
	ship.pos_y			= 124;
	ship.direction		= 0;
	ship.ship_tiles		= 4;
	ship.shoot_status	= 0;
	ship.bullet_tile	= 4;
	ship.max_bullets	= 5;
	ship.active_bullets	= 0;
	ship.bullet_timer	= 0;
	ship.bullet_trigger = 25;

	checkDirection();
	moveShip();
}

void createBullets() {
	UINT8 a;
	// create the bullets and assign the tiles
	for(a = 0; a < ship.max_bullets; a++) {
		bullets[a].tile = ship.bullet_tile + a;
		bullets[a].pos_x = 0;
		bullets[a].pos_y = 0;
		bullets[a].shoot = 0;

		set_sprite_tile(bullets[a].tile,13);
	}
}

void checkDirection() {
	UINT8 tile_position;

	for(ship.iter_tile = 0; ship.iter_tile < ship.ship_tiles; ship.iter_tile++) {
		tile_position = ship.iter_tile;
		if(ship.direction > 0) { tile_position += (4 * ship.direction); }
		set_sprite_tile(ship.iter_tile,tile_position);
	}
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

void checkShoot() {
	if(ship.shoot_status == 1) {
		if((ship.bullet_timer == 0 || ship.bullet_timer >= ship.bullet_trigger) && ship.active_bullets < ship.max_bullets) {
			bullets[ship.active_bullets].shoot = 1;
			bullets[ship.active_bullets].pos_x = ship.pos_x + 4;
			bullets[ship.active_bullets].pos_y = ship.pos_y - 8;
			ship.active_bullets++;

			if(ship.active_bullets == ship.max_bullets) {
				ship.active_bullets = 0;
			}
			
			ship.bullet_timer = 0;
		}
		ship.bullet_timer++;
	} else {
		ship.bullet_timer = 0;
	}
}

void moveBullets() {
	UINT8 a;
	for(a = 0; a <= 5; a++) {
		if(bullets[a].shoot == 1) {
			bullets[a].pos_y -= 3;

			if(bullets[a].pos_y > 0 && bullets[a].pos_y < 200) {
				move_sprite(bullets[a].tile,bullets[a].pos_x,bullets[a].pos_y);
			} else {
				bullets[a].shoot = 0;
			}
		}
	}
}

void moveBkg() {
	scroll_bkg(0,-1); 
}

void updateSwitches() {
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
}

void checkInput() {

	j = joypad();

	ship.shoot_status = (j & J_A) ? 1 : 0;  // A = shoot
	ship.direction = (j & J_RIGHT) ? 1 : (j & J_LEFT) ? 2 : 0; // check ship direction

	if ((j & J_UP) && ship.pos_y > 24)		{ ship.pos_y--; } // UP
	if ((j & J_RIGHT) && ship.pos_x < 144)	{ ship.pos_x++; } // RIGHT
	if ((j & J_DOWN) && ship.pos_y < 136)	{ ship.pos_y++; } // DOWN
	if ((j & J_LEFT) && ship.pos_x > 18)	{ ship.pos_x--; } // LEFT

	checkShoot();
	checkDirection();
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
