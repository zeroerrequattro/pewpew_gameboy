#include <gb/gb.h>

typedef struct {
	UINT8 pos_x;
	UINT8 pos_y;
	UINT8 direction;
	UINT8 ship_tiles;
	UINT8 iter_tile;
	UINT8 lives;
	UINT8 shoot_status;
	UINT8 bullet_tile;
	UINT8 max_bullets;
	UINT8 bullet_power;
	UINT8 active_bullets;
	UINT8 bullet_count;
	UINT8 bullet_limit;
} ship_struct;
