/*
 * graphics.c
 *
 *  Created on: Feb 12, 2023
 *      Author: Angad
 */

#include "../graphics/graphics.h"

graphics_context createGC(int stride, u8* frame) {

	graphics_context gc;
	gc.stride = stride;
	gc.frame = frame;
	return gc;

}

void drawPixel(int x, int y, int r, int g, int b, graphics_context *gc) {

	int iPixelAddr = x + (gc->stride * y);
	gc->frame[iPixelAddr] = r;
	gc->frame[iPixelAddr + 1] = g;
	gc->frame[iPixelAddr + 2] = b;

}

void drawRect(int x_start, int y_start, int width, int height, int r, int g,
		int b, graphics_context *gc) {

	int iPixelAddr;
	for (int x = (x_start * 3); x < (x_start + width) * 3; x = x + 3) {
		iPixelAddr = x + (gc->stride * y_start);
		for (int y = 0; y < height; y = y + 1) {
			gc->frame[iPixelAddr] = g; // green
			gc->frame[iPixelAddr + 1] = b; // blue
			gc->frame[iPixelAddr + 2] = r; // red
			iPixelAddr += gc->stride;
		}

	}

}

void drawBuffer(int x, int y, int draw_width, int draw_height, char* bufPtr, graphics_context *gc) {

	for (int xcoi = 0; xcoi < (draw_width * 3); xcoi += 3) {

		int iPixelAddr = xcoi + gc->stride*y;

		for (int ycoi = 0; ycoi < draw_height; ycoi++) {

			int addr = (xcoi / 3) + draw_width * ycoi;
			int offset = 255;

			int red = ((((bufPtr[addr] & 0xE0) >> 5) / 7)) * offset;
			int green = ((((bufPtr[addr] & 0x1C) >> 2)) / 7) * offset;
			int blue = ((bufPtr[addr] & 0x3) / 3) * offset;

			gc->frame[iPixelAddr] = green; // green
			gc->frame[iPixelAddr + 1] = blue; // blue
			gc->frame[iPixelAddr + 2] = red; // red

			iPixelAddr += gc->stride;
		}

	}

}

