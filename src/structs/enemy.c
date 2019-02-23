#include <gb/gb.h>

typedef struct {
    UINT8 tile;
    UINT8 pos_x;
    UINT8 pos_y;
    UINT8 visible;
    UINT8 type;
    UINT8 points;
    UINT8 health;
    UINT8 speed;
    UINT8 wait_time;
    UINT8 wait_position;
    UINT8 repeat;
} enemy_struct;