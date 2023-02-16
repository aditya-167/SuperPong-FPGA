#include "draw.h"
#include "../assets/test_image.h"

void DemoPrintTest(u8 *frame, u32 width, u32 height, u32 stride, int pattern) {

	switch (pattern) {

	case DEMO_PATTERN_0:

		DemoPrintTest0(frame, width, height, stride, pattern);
		break;

	case DEMO_PATTERN_1:

		DemoPrintTest1(frame, width, height, stride, pattern);
		break;

	case DEMO_PATTERN_2:

		DemoPrintTest2(frame, width, height, stride, pattern);
		break;

	case DEMO_PATTERN_3:

		DemoPrintTest3(frame, width, height, stride, pattern);
		break;

	default:
		xil_printf("Error: invalid pattern passed to DemoPrintTest");

	}
}

void DemoPrintTest0(u8 *frame, u32 width, u32 height, u32 stride, int pattern) {

	u32 xcoi, ycoi;
	u32 iPixelAddr;
	u8 wRed, wBlue, wGreen;
	double fRed, fBlue, fGreen;
	u32 xLeft, xMid, xRight, xInt;
	u32 yMid, yInt;
	double xInc, yInc;

	xInt = width / 4; //Four intervals, each with width/4 pixels
	xLeft = xInt * 3;
	xMid = xInt * 2 * 3;
	xRight = xInt * 3 * 3;
	xInc = 256.0 / ((double) xInt); //256 color intensities are cycled through per interval (overflow must be caught when color=256.0)

	yInt = height / 2; //Two intervals, each with width/2 lines
	yMid = yInt;
	yInc = 256.0 / ((double) yInt); //256 color intensities are cycled through per interval (overflow must be caught when color=256.0)

	fBlue = 0.0;
	fRed = 256.0;
	for (xcoi = 0; xcoi < (width * 3); xcoi += 3) {
		/*
		 * Convert color intensities to integers < 256, and trim values >=256
		 */
		wRed = (fRed >= 256.0) ? 255 : ((u8) fRed);
		wBlue = (fBlue >= 256.0) ? 255 : ((u8) fBlue);
		iPixelAddr = xcoi;
		fGreen = 0.0;
		for (ycoi = 0; ycoi < height; ycoi++) {

			wGreen = (fGreen >= 256.0) ? 255 : ((u8) fGreen);
			frame[iPixelAddr] = wRed;
			frame[iPixelAddr + 1] = wBlue;
			frame[iPixelAddr + 2] = wGreen;
			if (ycoi < yMid) {
				fGreen += yInc;
			} else {
				fGreen -= yInc;
			}

			/*
			 * This pattern is printed one vertical line at a time, so the address must be incremented
			 * by the stride instead of just 1.
			 */
			iPixelAddr += stride;
		}

		if (xcoi < xLeft) {
			fBlue = 0.0;
			fRed -= xInc;
		} else if (xcoi < xMid) {
			fBlue += xInc;
			fRed += xInc;
		} else if (xcoi < xRight) {
			fBlue -= xInc;
			fRed -= xInc;
		} else {
			fBlue += xInc;
			fRed = 0;
		}
	}
	/*
	 * Flush the framebuffer memory range to ensure changes are written to the
	 * actual memory, and therefore accessible by the VDMA.
	 */
	Xil_DCacheFlushRange((unsigned int ) frame, DEMO_MAX_FRAME);

}

void DemoPrintTest1(u8 *frame, u32 width, u32 height, u32 stride, int pattern) {

	u32 xcoi, ycoi;
	u32 iPixelAddr;
	u8 wRed, wBlue, wGreen;
	u32 wCurrentInt;
	double fColor;
	u32 xInt;
	double xInc;

	xInt = width / 7; //Seven intervals, each with width/7 pixels
	xInc = 256.0 / ((double) xInt); //256 color intensities per interval. Notice that overflow is handled for this pattern.

	fColor = 0.0;
	wCurrentInt = 1;
	for (xcoi = 0; xcoi < (width * 3); xcoi += 3) {

		/*
		 * Just draw white in the last partial interval (when width is not divisible by 7)
		 */
		if (wCurrentInt > 7) {
			wRed = 255;
			wBlue = 255;
			wGreen = 255;
		} else {
			if (wCurrentInt & 0b001)
				wRed = (u8) fColor;
			else
				wRed = 0;

			if (wCurrentInt & 0b010)
				wBlue = (u8) fColor;
			else
				wBlue = 0;

			if (wCurrentInt & 0b100)
				wGreen = (u8) fColor;
			else
				wGreen = 0;
		}

		iPixelAddr = xcoi;

		for (ycoi = 0; ycoi < height; ycoi++) {
			frame[iPixelAddr] = wRed;
			frame[iPixelAddr + 1] = wBlue;
			frame[iPixelAddr + 2] = wGreen;
			/*
			 * This pattern is printed one vertical line at a time, so the address must be incremented
			 * by the stride instead of just 1.
			 */
			iPixelAddr += stride;
		}

		fColor += xInc;
		if (fColor >= 256.0) {
			fColor = 0.0;
			wCurrentInt++;
		}
	}
	/*
	 * Flush the framebuffer memory range to ensure changes are written to the
	 * actual memory, and therefore accessible by the VDMA.
	 */
	Xil_DCacheFlushRange((unsigned int ) frame, DEMO_MAX_FRAME);

	/*
	 * Flush the framebuffer memory range to ensure changes are written to the
	 * actual memory, and therefore accessible by the VDMA.
	 */
	Xil_DCacheFlushRange((unsigned int ) frame, DEMO_MAX_FRAME);

}

void DemoPrintTest2(u8 *frame, u32 width, u32 height, u32 stride, int pattern) {

	graphics_context myGC = createGC(stride, frame);

	int px = 0;

	for (int i = 0; i < 500; i += 10) {

		drawRect(px, px, 10, 10, 0, 0, 0, &myGC);
		drawRect(i, i, 10, 10, 255, 255, 255, &myGC);
		Xil_DCacheFlushRange((unsigned int ) frame, DEMO_MAX_FRAME);
		px = i;
		usleep(33333 * 4);

	}

	Xil_DCacheFlushRange((unsigned int ) frame, DEMO_MAX_FRAME);

}

void DemoPrintTest3(u8 *frame, u32 width, u32 height, u32 stride, int pattern) {

	int xcoi, ycoi;
	int iPixelAddr;
	int draw_width = 640;
	int draw_height = 480;

	for (xcoi = 0; xcoi < (draw_width * 3); xcoi += 3) {

		iPixelAddr = xcoi;

		for (ycoi = 0; ycoi < draw_height; ycoi++) {

			int addr = (xcoi / 3) + draw_width * ycoi;
			int offset = 255;

			int red = ((((arr[addr] & 0xE0) >> 5) / 7)) * offset;
			int green = ((((arr[addr] & 0x1C) >> 2)) / 7) * offset;
			int blue = ((arr[addr] & 0x3) / 3) * offset;
			frame[iPixelAddr] = green; // green
			frame[iPixelAddr + 1] = blue; // blue
			frame[iPixelAddr + 2] = red; // red

			iPixelAddr += stride;
		}

	}

	Xil_DCacheFlushRange((unsigned int ) frame, DEMO_MAX_FRAME);

}
