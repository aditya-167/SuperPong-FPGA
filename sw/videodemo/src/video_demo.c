/************************************************************************/
/*																		*/
/*	video_demo.c	--	Nexys Video HDMI demonstration 						*/
/*																		*/
/************************************************************************/
/*	Author: Sam Bobrowicz												*/
/*	Copyright 2015, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*		This file contains code for running a demonstration of the		*/
/*		Video input and output capabilities on the Nexys Video. It is a good	*/
/*		example of how to properly use the display_ctrl and				*/
/*		video_capture drivers.											*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/* 																		*/
/*		11/25/2015(SamB): Created										*/
/*		03/31/2017(ArtVVB): Updated sleep functions for 2016.4			*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "video_demo.h"
#include "video_capture/video_capture.h"
#include "display_ctrl/display_ctrl.h"
#include "intc/intc.h"
#include <stdio.h>
#include "xuartlite_l.h"
#include "math.h"
#include <ctype.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "sleep.h"
#include "./assets/paused_image.h"
#include "graphics/graphics.h"
/*
 * Joy Stick Includes
 */
#include <stdio.h>
#include "xil_printf.h"
#include "PmodJSTK2.h"

/*
 * XPAR redefines
 */
#define DYNCLK_BASEADDR XPAR_AXI_DYNCLK_0_BASEADDR
#define VGA_VDMA_ID XPAR_AXIVDMA_0_DEVICE_ID
#define DISP_VTC_ID XPAR_VTC_0_DEVICE_ID
#define VID_VTC_ID XPAR_VTC_1_DEVICE_ID
#define VID_GPIO_ID XPAR_AXI_GPIO_VIDEO_DEVICE_ID
#define VID_VTC_IRPT_ID XPAR_INTC_0_VTC_1_VEC_ID
#define VID_GPIO_IRPT_ID XPAR_INTC_0_GPIO_0_VEC_ID
#define SCU_TIMER_ID XPAR_AXI_TIMER_0_DEVICE_ID
#define UART_BASEADDR XPAR_UARTLITE_0_BASEADDR

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/*
 * Display and Video Driver structs
 */
DisplayCtrl dispCtrl;
XAxiVdma vdma;
VideoCapture videoCapt;
INTC intc;
char fRefresh; //flag used to trigger a refresh of the Menu on video detect

/*
 * Framebuffers for video data
 */
u8 frameBuf[DISPLAY_NUM_FRAMES][DEMO_MAX_FRAME];
u8 *pFrames[DISPLAY_NUM_FRAMES]; //array of pointers to the frame buffers

/*
 * Interrupt vector table
 */
const ivt_t ivt[] = {
videoGpioIvt(VID_GPIO_IRPT_ID, &videoCapt),
videoVtcIvt(VID_VTC_IRPT_ID, &(videoCapt.vtc)) };

int test = 0;

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

int main(void) {

	Xil_ICacheEnable();
	Xil_DCacheEnable();

	DemoInitialize();

	DemoRun();

	Xil_DCacheDisable();
	Xil_ICacheDisable();
	return 0;
}

void DemoInitialize() {
	int Status;
	XAxiVdma_Config *vdmaConfig;
	int i;

	/*
	 * Initialize an array of pointers to the 3 frame buffers
	 */
	for (i = 0; i < DISPLAY_NUM_FRAMES; i++) {
		pFrames[i] = frameBuf[i];
	}

	/*
	 * Initialize VDMA driver
	 */
	vdmaConfig = XAxiVdma_LookupConfig(VGA_VDMA_ID);
	if (!vdmaConfig) {
		xil_printf("No video DMA found for ID %d\r\n", VGA_VDMA_ID);
		return;
	}
	Status = XAxiVdma_CfgInitialize(&vdma, vdmaConfig, vdmaConfig->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("VDMA Configuration Initialization failed %d\r\n", Status);
		return;
	}

	/*
	 * Initialize the Display controller and start it
	 */
	Status = DisplayInitialize(&dispCtrl, &vdma, DISP_VTC_ID, DYNCLK_BASEADDR,
			pFrames, DEMO_STRIDE);
	if (Status != XST_SUCCESS) {
		xil_printf(
				"Display Ctrl initialization failed during demo initialization%d\r\n",
				Status);
		return;
	}
	Status = DisplayStart(&dispCtrl);
	if (Status != XST_SUCCESS) {
		xil_printf("Couldn't start display during demo initialization%d\r\n",
				Status);
		return;
	}

	/*
	 * Initialize the Interrupt controller and start it.
	 */
	Status = fnInitInterruptController(&intc);
	if (Status != XST_SUCCESS) {
		xil_printf("Error initializing interrupts");
		return;
	}
	fnEnableInterrupts(&intc, &ivt[0], sizeof(ivt) / sizeof(ivt[0]));

	return;
}

void DemoRun() {

	// Greeting message
	PrintMenu();

	char userInput = 0;
	u32 locked;
	XGpio *GpioPtr = &videoCapt.gpio;

	DemoPrintTest(pFrames[dispCtrl.curFrame], dispCtrl.vMode.width,
			dispCtrl.vMode.height, DEMO_STRIDE, DEMO_PATTERN_3);

	/* Flush UART FIFO */
	while (!XUartLite_IsReceiveEmpty(UART_BASEADDR)) {
		XUartLite_ReadReg(UART_BASEADDR, XUL_RX_FIFO_OFFSET);
	}

	while (userInput != 'q') {
		fRefresh = 0;

		/* Wait for data on UART */
		while (XUartLite_IsReceiveEmpty(UART_BASEADDR) && !fRefresh) {
		}

		/* Store the first character in the UART receive FIFO and echo it */
		if (!XUartLite_IsReceiveEmpty(UART_BASEADDR)) {
			userInput = XUartLite_ReadReg(UART_BASEADDR, XUL_RX_FIFO_OFFSET);
			xil_printf("You pressed: %c\n", userInput);
		} else  //Refresh triggered by video detect interrupt
		{
			userInput = 'r';
		}

		switch (userInput) {
		case '1':
			GameLoop(pFrames[dispCtrl.curFrame], dispCtrl.vMode.width,
					dispCtrl.vMode.height, DEMO_STRIDE);
			break;
		case '2':
			DemoPrintTest(pFrames[dispCtrl.curFrame], dispCtrl.vMode.width,
					dispCtrl.vMode.height, DEMO_STRIDE, DEMO_PATTERN_3);
			break;
		case '3':
			DemoPrintTest(pFrames[dispCtrl.curFrame], dispCtrl.vMode.width,
					dispCtrl.vMode.height, DEMO_STRIDE, DEMO_PATTERN_0);
			break;
		case '4':
			DemoPrintTest(pFrames[dispCtrl.curFrame], dispCtrl.vMode.width,
					dispCtrl.vMode.height, DEMO_STRIDE, DEMO_PATTERN_1);
			break;
		case '5':
			DemoPrintTest(pFrames[dispCtrl.curFrame], dispCtrl.vMode.width,
					dispCtrl.vMode.height, DEMO_STRIDE, DEMO_PATTERN_2);
			break;
		case '6':
			DemoPrintTest(pFrames[dispCtrl.curFrame], dispCtrl.vMode.width,
					dispCtrl.vMode.height, DEMO_STRIDE, 4);
			break;
		case '7':
			JoyStickDemo();
			break;
		case 'q':
			break;
		case 'r':
			locked = XGpio_DiscreteRead(GpioPtr, 2);
			xil_printf("%d", locked);
			break;
		default:
			usleep(50000);
		}
	}

	return;
}

void GameLoop(u8 *frame, u32 width, u32 height, u32 stride) {

	// Setup joysticks
	PmodJSTK2 joystick;
	// Initialise joystick
	JSTK2_begin(&joystick, XPAR_PMODJSTK2_0_AXI_LITE_SPI_BASEADDR);
	// Set no inversion
	JSTK2_setInversion(&joystick, 0, 1);

	// Setup and initialisation
	graphics_context gc;
	gc.frame = frame;
	gc.stride = stride;

	clearScreen(&gc);

	game_context g;
	initialize(&g, &joystick, NULL);

	XTmrCtr TimerCounter;
	setup_stopwatch(&TimerCounter);

	int a = 0, b = 0;
	xil_printf("Starting the game. Game STATE = %d\n", g.state);

	// Choose the player's colours
	int colours[3] = { 0, 0, 0 };
	chooseColours(&joystick, &(colours[0]), &gc, &g);

	// Clear screen again
	clearScreen(&gc);

	// Set colours for the players
	(g.pad1)->r = colours[0];
	(g.pad1)->g = colours[1];
	(g.pad1)->b = colours[2];

	while (g.state != STOPPED) {

		a = start_stopwatch(&TimerCounter);
		update(1, &g, &gc);
		Xil_DCacheFlushRange((unsigned int ) frame, DEMO_MAX_FRAME);
		b = end_stopwatch(&TimerCounter);

		float time_to_wait = (float) ((b - a) * 0.00001f) * 1000;
		usleep(33333 - (int) time_to_wait);

	}

	drawFullScreenImage(paused_image, &gc);
	shutdown(&g);
}

void PrintMenu() {

	xil_printf("\x1B[H"); //Set cursor to top left of terminal
	xil_printf("\x1B[2J"); //Clear terminal
	xil_printf("**************************************************\n\r");
	xil_printf("*             PONG SUPERIOR GAME              *\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("1 - Play Game\n\r");
	xil_printf("2 - Demo 1\n\r");
	xil_printf("3 - Demo 2\n\r");
	xil_printf("4 - Demo 3\n\r");
	xil_printf("5 - Demo 4\n\r");
	xil_printf("6 - Demo 5\n\r");
	xil_printf("7 - Test Joysticks\n\r");
	xil_printf("q - Quit\n\r");
	xil_printf("\n\r");
	xil_printf("\n\r");
	xil_printf("Enter a selection:");

}

int chooseColours(PmodJSTK2* joystick, int* colours, graphics_context* gc, game_context* game) {

	int stop = 0;
	int r = 10;
	int g = 10;
	int b = 10;

	DemoPrintTest(gc->frame, 1, 1, DEMO_STRIDE, 5);

	while (!stop) {

		// Get coord values
		int Ydata = JSTK2_getY(joystick);
		int Xdata = JSTK2_getX(joystick);

		xil_printf("\n\rX: %d  Y: %d BTN: %x", Xdata, Ydata,
				JSTK2_getBtns(joystick));
		delay(5000);

		float yStep;
		yStep = JOY_STEP * (((float) (Ydata - 128)) / 128);

		float xStep;
		xStep = JOY_STEP * (((float) (Xdata - 128)) / 128);

		r += yStep;
		g += xStep;

		// Draw coloured rectangle
		drawRect(136, 331, 397, 81, r, g, b, gc);
		// Flush screen buffer
		Xil_DCacheFlushRange((unsigned int ) gc->frame, DEMO_MAX_FRAME);

		// Set led from btns and axis
		if (JSTK2_getBtns(joystick) == 2)
			stop = 1;
		else if (JSTK2_getBtns(joystick) == 1)
			b++;
		else
			JSTK2_setLed(joystick, r, g, b);

	}

	*colours = r;
	*(colours + 1) = g;
	*(colours + 2) = b;

	return 0;

}

int JoyStickDemo() {

	u16 Ydata;
	u16 Xdata;

	PmodJSTK2 joysticks;
	PmodJSTK2* joystick = &joysticks;

	//init joystick
	JSTK2_begin(joystick, XPAR_PMODJSTK2_0_AXI_LITE_SPI_BASEADDR);

	//set no inversion
	JSTK2_setInversion(joystick, 0, 1);

	xil_printf("\n\rJoystick Demo\n");

	int stop = 0;

	while (!stop) {

		// Get coord values
		Ydata = JSTK2_getY(joystick);
		Xdata = JSTK2_getX(joystick);
		xil_printf("\n\rX: %d     Y: %d	   BTN: %x", Xdata, Ydata,
				JSTK2_getBtns(joystick));
		delay(5000);

		// Set colour on screen

		// Set led from btns and axis
		if (JSTK2_getBtns(joystick) == 1)
			JSTK2_setLed(joystick, 0, 255, 0);
		else if (JSTK2_getBtns(joystick) == 2)
			stop = 1;
		else
			JSTK2_setLed(joystick, Xdata, 0, Ydata);

	}

	return 0;
}
