/*
 * graphics.h
 *
 *  Created on: Feb 12, 2023
 *      Author: Angad
 */
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

typedef struct GraphicsContext GraphicsContext;

// Aggregation of some graphics data
struct GraphicsContext {

	int stride;
	u8 *frame;

};

// Pack GC data for easier access into a struct
GraphicsContext createGC(int stride, u8* frame);

// Draw Rectangle at x_start, y_start
void drawRect(int x_start, int y_start, int width, int height, int r, int g, int b, GraphicsContext *gc);

// Draw Pixel at co-ordinates specified by x and y of colour (r, g, b).
void drawPixel(int x,int y,int r,int g,int b, GraphicsContext *gc);


// Draw pixels based on a specific buffer
void drawBuffer(int x, int y, int width, int height, char* bufPtr, GraphicsContext *gc);

