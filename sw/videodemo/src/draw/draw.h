#ifndef DRAW_H
#define DRAW_H

#include "xil_types.h"
#include <stdio.h>
#include "xuartlite_l.h"
#include "../graphics/graphics.h"
#include "math.h"
#include <ctype.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "sleep.h"
#include "../jpg/nanojpg.h"

/* ------------------------------------------------------------ */
/*					Custom Includes    							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#define DEMO_MAX_FRAME (1920*1080*3)
#define DEMO_STRIDE (1920 * 3)
#define DEMO_PATTERN_0 0
#define DEMO_PATTERN_1 1
#define DEMO_PATTERN_2 2
#define DEMO_PATTERN_3 3
#define DEMO_PATTERN_FLEXO 90

// Colour Structure
typedef struct Color16 {
	int r;
	int g;
	int b;
} Color16;

// Drawing functionality
void DemoPrintTest(u8 *frame, u32 width, u32 height, u32 stride, int pattern);

//Draw Test 0 
void DemoPrintTest0(u8 *frame, u32 width, u32 height, u32 stride, int pattern);

//Draw Test 1
void DemoPrintTest1(u8 *frame, u32 width, u32 height, u32 stride, int pattern);

// Draw Test 2
void DemoPrintTest2(u8 *frame, u32 width, u32 height, u32 stride, int pattern);

// Draw Test 3
void DemoPrintTest3(u8 *frame, u32 width, u32 height, u32 stride, int pattern);

// Draw Test 4
void DemoPrintTest4(u8 *frame, u32 width, u32 height, u32 stride, int pattern);

// Draw Test 5
void DemoPrintTest5(u8 *frame, u32 width, u32 height, u32 stride, int pattern);

// Draw Test 6
void DemoPrintTest6(u8 *frame, u32 width, u32 height, u32 stride, int pattern);

// Draw Test 6
void DemoPrintTestFlexo(u8 *frame, u32 width, u32 height, u32 stride, int pattern);

// Get RGB16 values
void getRGB16(int number, Color16* clrPtr);
#endif
