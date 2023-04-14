/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "video_demo.h"
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

/*
 * XPAR redefines
 */

#define UART_BASEADDR XPAR_UARTLITE_0_BASEADDR

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

INTC intc;
char fRefresh; //flag used to trigger a refresh of the Menu on video detect

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

int main(void) {

	Xil_ICacheEnable();
	Xil_DCacheEnable();

	DemoRun();

	Xil_DCacheDisable();
	Xil_ICacheDisable();
	return 0;
}

void DemoRun() {

	int stop = 0;

	/* Flush UART FIFO */
	while (!XUartLite_IsReceiveEmpty(UART_BASEADDR)) {
		XUartLite_ReadReg(UART_BASEADDR, XUL_RX_FIFO_OFFSET);
	}

	while (!stop) {

		xil_printf("%d\n", get_number());

	}

	return;
}

