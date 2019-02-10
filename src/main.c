#include <gb/gb.h>
#include <gb/font.h>
//#include <rand.h>

#include "structs/ship.c"
#include "structs/bullet.c"
#include "structs/text.c"

#include "sprites/bkgSprites.c"
#include "sprites/sprites.c"
#include "sprites/font.c"

#include "tiles/bkgFloor.c"
#include "tiles/display.c"

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

//UINT8 limRand(UINT8, UINT8);
//UINT8 collisionCheck(UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8);

UINT8 x,y,i,j,bkg_y,enemy_timer,score;

ship_struct ship;
bullet_struct bullets[5];
text_struct text;

void main() {
	init();

	OBP0_REG = 0xD0; //0x1B; 0xE1; change sprite palette 
	//OBP1_REG = 0x1B;

	while(1) {
		checkInput();     // Check for user input (and act on it)
		updateSwitches(); // Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		moveBkg();
        wait_vbl_done();  // Wait until VBLANK to avoid corrupting visual memory
		delay(10);
	}
}

void init() {

	LCDC_REG = 0x63; // 01100011
	/*
	* LCD        = Off		0
	* WindowBank = 0x9C00	1
	* Window     = On		1
	* BG Chr     = 0x8800	0
	* BG Bank    = 0x9800	0
	* OBJ        = 8x8		0
	* OBJ        = On		1
	* BG         = On		1
	*/

	disable_interrupts();

	DISPLAY_OFF;
	
    SHOW_BKG;
	SHOW_WIN;
    SHOW_SPRITES;
    
	font_init();

	enable_interrupts();

	set_bkg_data(0,18,bkgSprites);
	set_win_data(18,42,font);
	
	//set_win_tiles(0,0,21,2,display);

	WX_REG = 0x00;
	WY_REG = 0x80;

	for(y = 0; y < 9; y++) {
		set_bkg_tiles(0,((y*4)-4)+i,20,4,bkgFloor);
	}

	bkg_y = 0;
	score = 0;
	enemy_timer = 0;
	
	set_sprite_data(0,25,sprites);
	set_sprite_data(25,42,font);

	createShip();
	createBullets();

	DISPLAY_ON; // Turn on the display
}

void createShip() {
	ship.pos_x			= 80;
	ship.pos_y			= 124;
	ship.direction		= 0;
	ship.ship_tiles		= 4;
	ship.lives			= 1;
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
	// create the bullets and assign the tiles
	for(i = 0; i < ship.max_bullets; i++) {
		bullets[i].tile = ship.bullet_tile + i;
		bullets[i].pos_x = 0;
		bullets[i].pos_y = 0;
		bullets[i].shoot = 0;

		set_sprite_tile(bullets[i].tile,13);
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
	for(i = 0; i <= 5; i++) {
		if(bullets[i].shoot == 1) {
			bullets[i].pos_y -= 3;

			if(bullets[i].pos_y > 0 && bullets[i].pos_y < 200) {
				move_sprite(bullets[i].tile,bullets[i].pos_x,bullets[i].pos_y);
			} else {
				bullets[i].shoot = 0;
			}
		}
	}
}

void moveBkg() {
	scroll_bkg(0,-1);
}

void updateSwitches() {
    SHOW_BKG;
	SHOW_WIN;
    SHOW_SPRITES;
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
