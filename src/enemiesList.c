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
 *      5: visible (must be 0),
 *      6: speed (must be >= 1),
 *      7: wait time (0 to make it go straight down)
 *      8: Y position where the enemy stops
 *      9: # of repeated entrance (0 = no repeat)
 * }
 */

UINT8 enemiesList[6][3][10] = {
    // 1: normal wave, 1 appearance
    {
        { 0,2,100,52,0,0,1,5,80,0 },
        { 0,2,100,84,8,0,1,5,80,0 },
        { 0,2,100,116,0,0,1,5,80,0 }
    },
    // 2: fast enemies, 4 times appearance
    {
        { 0,1,50,60,0,0,2,0,0,3 },
        { 0,1,50,84,8,0,2,0,0,3 },
        { 0,1,50,108,0,0,2,0,0,3 }
    },
    // 3: normal wave
    {
        { 0,2,100,52,8,0,1,3,80,1 },
        { 0,2,100,84,0,0,1,3,80,1 },
        { 0,2,100,116,8,0,1,3,80,1 }
    },
    // 4: fast wave, 1 appearance
    {
        { 0,1,50,52,0,0,2,0,80,0 },
        { 0,1,50,84,4,0,2,0,80,0 },
        { 0,1,50,116,8,0,2,0,80,0 }
    },
    // 5: fast wave, 1 appearance
    {
        { 0,1,50,52,8,0,2,0,80,0 },
        { 0,1,50,84,4,0,2,0,80,0 },
        { 0,1,50,116,0,0,2,0,80,0 }
    },
    // 6: fast wave, 1 appearance
    {
        { 0,1,50,52,8,0,2,0,80,0 },
        { 0,1,50,84,0,0,2,0,80,0 },
        { 0,1,50,116,8,0,2,0,80,0 }
    },
    // 7: though wave, 2 appearance
    {
        { 0,5,200,52,0,0,1,3,80,1 },
        { 0,5,200,84,0,0,1,3,80,1 },
        { 0,5,200,116,0,0,1,3,80,1 }
    }
};
