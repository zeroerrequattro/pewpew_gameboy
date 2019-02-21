#include <gb/gb.h>

/**
 * Array count.
 * use UWORD instead of size_t
 */
UWORD count( UINT8[] );

UWORD count( UINT8 array[] ) {
    return ( sizeof(array) / sizeof(UINT8) );
}