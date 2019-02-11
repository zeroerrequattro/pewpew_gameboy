#include <gb/gb.h>
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
void initScore();
void locateFontTiles();
void createShip();
void createBullets();
void checkInput();
void checkDirection();
void moveShip();
void checkShoot();
void moveBullets();
void moveBkg();
void updateSwitches();
void updateLives();


void updateScore( UINT8 );
//UINT8 limRand(UINT8, UINT8);
//UINT8 collisionCheck(UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8);

UINT8 x,y,i,j,score_tile,digit,max_score,enemy_timer,multiplier,font_tiles[36],lives;
UINT32 score;	
UINT32 tmp_score;

ship_struct ship;
bullet_struct bullets[5];
text_struct text;

void main() {
	init();

	OBP0_REG = 0xD0; //0x1B; 0xE1; change sprite palette 
	//OBP1_REG = 0x1B;

	while(1) {
		moveBkg();
		checkInput();     // Check for user input (and act on it)
		updateSwitches(); // Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
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

	enable_interrupts();

	set_bkg_data(0,18,bkgSprites);
	set_win_data(18,42,font);
	
	set_win_tiles(0,0,21,2,display);

	//WX_REG = 0x00;
	WY_REG = 0x80;

	for(y = 0; y < 9; y++) {
		set_bkg_tiles(0,((y*4)-4)+i,20,4,bkgFloor);
	}

	score = 0;
	enemy_timer = 0;
	lives = 2;
	score_tile = 9;

	locateFontTiles();

	set_sprite_data(0,25,sprites);
	set_sprite_data(25,42,font);

	initScore();
	updateLives();

	// score sprites
	move_sprite(9,0x98,0x94);
	move_sprite(10,0x90,0x94);
	move_sprite(11,0x88,0x94);
	move_sprite(12,0x80,0x94);
	move_sprite(13,0x78,0x94);

	// lives sprites
	move_sprite(14,0x0D,0x95);
	move_sprite(15,0x16,0x95);

	createShip();
	createBullets();

	DISPLAY_ON; // Turn on the display
}

void locateFontTiles() {
	for (i = 0; i < 36; i++) {
		font_tiles[i] = 25 + i;
	}
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

void checkInput() {

	j = joypad();

	ship.shoot_status = (j & J_A) ? 1 : 0;  // A = shoot
	ship.direction = (j & J_RIGHT) ? 1 : (j & J_LEFT) ? 2 : 0; // check ship direction

	if(j & J_B) { updateScore(2); }
	if ((j & J_UP) && ship.pos_y > 24)		{ ship.pos_y--; } // UP
	if ((j & J_RIGHT) && ship.pos_x < 144)	{ ship.pos_x++; } // RIGHT
	if ((j & J_DOWN) && ship.pos_y < 136)	{ ship.pos_y++; } // DOWN
	if ((j & J_LEFT) && ship.pos_x > 18)	{ ship.pos_x--; } // LEFT

	checkShoot();
	checkDirection();
	moveBullets();
	moveShip();
}

void updateSwitches() {
    SHOW_BKG;
	SHOW_WIN;
    SHOW_SPRITES;
}

void initScore() {
	for (i = 9; i < 14; i++) {
		set_sprite_tile(i,25);
	}
}

void updateScore( UINT8 point ) {
	score += point;
	tmp_score = score;
	while (tmp_score > 0) {
		digit = tmp_score % 10;
		// do something with digit
		set_sprite_tile(score_tile,font_tiles[digit]);
		score_tile++;
		tmp_score /= 10;
	}
	score_tile = 9;
}

void updateLives() {
	for(i = 1; i < 3; i++) {
		set_sprite_tile((13 + i),(i <= lives) ? 18 : 65);
	}
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
