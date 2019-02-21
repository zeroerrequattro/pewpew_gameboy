#include <gb/gb.h>

/**
 * reference:
 * 25: tile # enemy01
 * 
 * this is a multidimensional array
 * It contains a number of arrays that is formatted as:
 * path[ incoming timer ] = { enemy type, health, points, initial pos x, initial pos y, visible }
 */

UINT8 enemiesList[1][3][6] = {
    {
        { 0,2,100,52,0,0 },
        { 0,2,100,84,4,0 },
        { 0,2,100,116,0,0 }
    }
};
