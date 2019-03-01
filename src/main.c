#include <gb/gb.h>

#include "enemiesList.c"

#include "structs/ship.c"
#include "structs/bullet.c"
#include "structs/enemy.c"

#include "sprites/bkgSprites.c"
#include "sprites/sprites.c"
#include "sprites/font.c"

#include "tiles/bkgFloor.c"
#include "tiles/display.c"

// declaration

void init();
void timer_isr();
void initScore();
void locateFontTiles();
void createShip();
void createBullets();
void checkInput();
void checkDirection();
void checkShoot();
void enemiesRoutine();
void moveBkg();
void moveShip();
void moveBullets();
void moveEnemies();
void updateSwitches();
void updateLives();

void updateScore( UINT8 );
UINT8 collisionCheck(UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8, UINT8);

UINT8 x,y,i,h,j,score_pos_x,score_tile,digit,max_score,enemy_timer,check_enemies,enemy_quota,multiplier,font_tiles[36],lives;
UINT32 score,tmp_score,timer,clock_counter;

ship_struct ship;
enemy_struct enemies[3];
bullet_struct bullets[5];

void main() {
	init();

	OBP0_REG = 0xD0; //0x1B; 0xE1; change sprite palette 
	//OBP1_REG = 0x1B;

	while(1) {
		moveBkg();
		enemiesRoutine();
		checkInput();     // Check for user input (and act on it)
		updateSwitches(); // Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
        wait_vbl_done();  // Wait until VBLANK to avoid corrupting visual memory
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
	/* Set TMA to divide clock by 0x100 */
	TMA_REG = 0x00U;
	/* Set clock to 4096 Hertz */
	TAC_REG = 0x04U;

	disable_interrupts();

	DISPLAY_OFF;
	
    SHOW_BKG;
	SHOW_WIN;
    SHOW_SPRITES;

    add_TIM(timer_isr);

	enable_interrupts();

	set_interrupts( VBL_IFLAG | TIM_IFLAG );

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
	enemy_quota = 0;
	multiplier = 1;
	lives = 2;
	score_tile = 9;

	locateFontTiles();

	set_sprite_data(0,29,sprites);
	set_sprite_data(29,42,font);

	initScore();
	updateLives();

	score_pos_x = 0x98;
	// position score sprites
	for(i = 9; i < 14; i++ ) {
		move_sprite(i,score_pos_x,0x94);
		score_pos_x -= 0x08;
	}

	// lives sprites
	move_sprite(14,0x0D,0x95);
	move_sprite(15,0x16,0x95);

	createShip();
	createBullets();

	DISPLAY_ON; // Turn on the display
}

void timer_isr() NONBANKED {
	clock_counter++;
	if(clock_counter % 16 == 0){
		timer++;
	}
}

void locateFontTiles() {
	for (i = 0; i < 36; i++) {
		font_tiles[i] = 29 + i;
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
	ship.bullet_power	= 1;
	ship.bullet_count	= 0;
	ship.bullet_limit 	= 25;

	for (i = 0; i < 4; i++) { set_sprite_tile(i,i); }
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
		if((ship.bullet_count == 0 || ship.bullet_count >= ship.bullet_limit) && ship.active_bullets < ship.max_bullets) {
			bullets[ship.active_bullets].shoot = 1;
			bullets[ship.active_bullets].pos_x = ship.pos_x + 4;
			bullets[ship.active_bullets].pos_y = ship.pos_y - 8;
			ship.active_bullets++;

			if(ship.active_bullets == ship.max_bullets) {
				ship.active_bullets = 0;
			}
			
			ship.bullet_count = 0;
		}
		ship.bullet_count++;
	} else {
		ship.bullet_count = 0;
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
	SCY_REG -= 1;
}

void enemiesRoutine() {
	if(timer > 3) {
		check_enemies = 1; // set checker to true;

		for (i = 0; i < 3; i++) {
			if(enemiesList[enemy_timer][i][5] == 0) {
				enemiesList[enemy_timer][i][5] = 1;
				enemies[i].tile			= 16 + i;
				enemies[i].type			= 25 + enemiesList[enemy_timer][i][0];
				enemies[i].health		= enemiesList[enemy_timer][i][1];
				enemies[i].points		= enemiesList[enemy_timer][i][2];
				enemies[i].pos_x		= enemiesList[enemy_timer][i][3];
				enemies[i].pos_y		= enemiesList[enemy_timer][i][4];
				enemies[i].visible		= timer;
				enemies[i].speed		= enemiesList[enemy_timer][i][6];
				enemies[i].wait_time	= enemiesList[enemy_timer][i][7];
				enemies[i].wait_position = enemiesList[enemy_timer][i][8];
				enemies[i].repeat		= enemiesList[enemy_timer][i][9];

				set_sprite_tile(enemies[i].tile,enemies[i].type);

			} else {
				
				if(
					( enemies[i].wait_time == 0 ||
						( enemies[i].pos_y < enemies[i].wait_position || 
					  	  timer > ( enemies[i].visible + enemies[i].wait_time ))
					) && enemies[i].visible != 0
				) {
					enemies[i].pos_y += enemies[i].speed;
				}

				if(enemies[i].pos_y > 144) {
					enemies[i].pos_y = 0;

					if(enemies[i].repeat > 0) {
						enemies[i].repeat--;
					} else {
						enemies[i].visible = 0;
					}
				}

				// check collisions to bullets and ship
				for(h = 0; h < ship.max_bullets; h++){
					if( // collision to bullets
						collisionCheck(
							enemies[i].pos_x,
							enemies[i].pos_y,
							8,
							8,
							bullets[h].pos_x,
							bullets[h].pos_y,
							8,
							8
						) == 1
						&& enemies[i].visible > 0
					) {
						bullets[h].pos_x = 0;
						enemies[i].health -= ship.bullet_power;

						if(enemies[i].health <= 0) {
							enemies[i].visible = 0;
							enemies[i].pos_y = 0;
							enemy_quota++;
							updateScore(enemies[i].points);
						}
					}

					if( // collision to ship
						collisionCheck(
							enemies[i].pos_x,
							enemies[i].pos_y,
							8,
							8,
							ship.pos_x,
							ship.pos_y,
							16,
							16
						) == 1
					) {
						enemies[i].visible = 0;
						enemies[i].pos_y = 0;
						multiplier = 1;
						lives--;
						updateLives();
					}
				}
			}

			if(enemies[i].visible > 0) { check_enemies = 0; }

			move_sprite((16 + i),enemies[i].pos_x,enemies[i].pos_y);
		}

		if(check_enemies == 1) {
			enemy_timer++;
		}
	}
}

void checkInput() {

	j = joypad();

	ship.shoot_status = (j & J_A) ? 1 : 0;  // A = shoot
	ship.direction = (j & J_RIGHT) ? 1 : (j & J_LEFT) ? 2 : 0; // check ship direction

	if ((j & J_UP) && ship.pos_y > 24)		{ ship.pos_y--; } // UP
	if ((j & J_RIGHT) && ship.pos_x < 144)	{ ship.pos_x++; } // RIGHT
	if ((j & J_DOWN) && ship.pos_y < 127)	{ ship.pos_y++; } // DOWN
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
		set_sprite_tile(i,29);
	}
}

void updateScore( UINT8 point ) {
	score += (point * multiplier);
	tmp_score = score;

	// split tmp_score by digits
	// and put them in right place
	while (tmp_score > 0) {
		digit = tmp_score % 10;

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

// Check if two rectangles from x1,y1, and extending out h1, h2, 
// overlap with another, x2,y2, and extending out w2, h2
UINT8 collisionCheck(UINT8 x1, UINT8 y1, UINT8 w1, UINT8 h1, UINT8 x2, UINT8 y2, UINT8 w2, UINT8 h2) {
	if ((x1 < (x2+w2)) && ((x1+w1) > x2) && (y1 < (h2+y2)) && ((y1+h1) > y2)) {
		return 1;
	} else {
		return 0;
	}
}
