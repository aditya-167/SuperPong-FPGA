#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "xil_types.h"
#include <stdio.h>
#include "xuartlite_l.h"
//#include "xuartps.h"
#include "math.h"
#include <ctype.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "sleep.h"

// Aggregation of some graphics data
typedef struct graphics_context{

	int stride;
	u8 *frame;

}graphics_context;

// Pack GC data for easier access into a struct
graphics_context createGC(int stride, u8* frame);

// Draw Rectangle at x_start, y_start
void drawRect(int x_start, int y_start, int width, int height, int r, int g, int b, graphics_context *gc);

// Draw Pixel at co-ordinates specified by x and y of colour (r, g, b).
void drawPixel(int x,int y,int r,int g,int b, graphics_context *gc);

// Draw pixels based on a specific buffer
void drawBuffer(int x, int y, int width, int height, char* bufPtr, graphics_context *gc);

#endif
