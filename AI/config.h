#ifdef HAS_DXL
#include <dxl.h>
#endif

/**
 * This is the dimension of the legs, in mm
 */
#define L0      35
#define L1      32
#define L2      60
#define L3_1    85
#define L3_2    35


/**
 * Chooses the initial orientation of the spider, set this to 1 if
 * you want it to be initialized in the opposite orientation
 */
static int initialOrientation = 0;

/**
 * You can also change the signs of the servos if you mount
 * it backward for instance
 *
 * Change the 1 to -1 if the servos are backwards
 */
static int signs[] = {
    1,          // Signs of the body motors
    -1,          // Sign of the middle-leg motors
    1           // Sign of the end-legs motors
};
