#include <gb/gb.h>

/**
 * reference:
 * 25: starting enemies tile
 * 
 * enemiesList is a multidimensional array.
 * It contains a number of arrays that is formatted as:
 * path[ incoming timer ] = {
 *      0: enemy type, 
 *      1: health,
 *      2: points,
 *      3: initial pos x,
 *      4: initial pos y,
 *      5: visible,
 *      6: speed (must be >= 1),
 *      7: wait time (0 to make it go straight down)
 *      8: Y position where the enemy stops
 *      9: # of repeated entrance (0 = no repeat)
 * }
 */

UINT8 enemiesList[6][3][10] = {
    {
        { 0,2,100,52,0,0,1,5,80,0 },
        { 0,2,100,84,8,0,1,5,80,0 },
        { 0,2,100,116,0,0,1,5,80,0 }
    },
    {
        { 0,1,50,60,0,0,2,0,0,3 },
        { 0,1,50,84,8,0,2,0,0,3 },
        { 0,1,50,108,0,0,2,0,0,3 }
    },
    {
        { 0,2,120,52,8,0,1,3,80,0 },
        { 0,2,120,84,0,0,1,3,80,0 },
        { 0,2,120,116,8,0,1,3,80,0 }
    },
    {
        { 0,2,120,52,8,0,1,3,80,0 },
        { 0,2,120,84,0,0,1,3,80,0 },
        { 0,2,120,116,8,0,1,3,80,0 }
    },
    {
        { 0,2,120,52,8,0,1,3,80,0 },
        { 0,2,120,84,0,0,1,3,80,0 },
        { 0,2,120,116,8,0,1,3,80,0 }
    },
    {
        { 0,2,120,52,8,0,1,3,80,0 },
        { 0,2,120,84,0,0,1,3,80,0 },
        { 0,2,120,116,8,0,1,3,80,0 }
    }
};
