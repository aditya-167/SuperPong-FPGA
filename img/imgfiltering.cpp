#include "imgfiltering.h"
#include "stdio.h"

#define HUE_HIGH 157;
#define HUE_LOW 110;

#define SAT_HIGH 255;
#define SAT_LOW 47;

#define	VAL_HIGH 111;
#define VAL_LOW 255;



void img_filter(hls::stream<uint_8_side_channel> &inStream,hls::stream<uint_8_side_channel> &outStream, ap_uint<3> sel){
	#pragma HLS INTERFACE axis port=inStream
	#pragma HLS INTERFACE axis port=outStream

	char kernel[KERNEL_DIM*KERNEL_DIM];

	switch(sel){
		case 0: {
			kernel[0] = 0; kernel[1] = 0; kernel[2] = 0;
			kernel[3] = 0; kernel[4] = 1; kernel[5] = 0;
			kernel[6] = 0; kernel[7] = 0; kernel[8] = 0;
			break;
		}
		case 1:{
			kernel[0] = -1; kernel[1] = -1; kernel[2] = -1;
			kernel[3] = -1; kernel[4] = 8; kernel[5] = -1;
			kernel[6] = -1; kernel[7] = -1; kernel[8] = -1;
			break;
		}
		case 2:{
			kernel[0] = -1; kernel[1] = -2; kernel[2] = -1;
			kernel[3] = 0; kernel[4] = 0; kernel[5] = 0;
			kernel[6] = 1; kernel[7] = 2; kernel[8] = 1;
			break;
		}
		default:{
			kernel[0] = 0; kernel[1] = 0; kernel[2] = 0;
			kernel[3] = 0; kernel[4] = 1; kernel[5] = 0;
			kernel[6] = 0; kernel[7] = 0; kernel[8] = 0;
			break;
		}
	}

	hls::LineBuffer<KERNEL_DIM,IMG_WIDTH_OR_COLS,unsigned char> lineBuff;
	hls::Window<KERNEL_DIM,KERNEL_DIM,short> window;

	int idxCol = 0;
	int idxRow = 0;
	int pixConvolved = 0;
	int waitTicks = (IMG_WIDTH_OR_COLS*(KERNEL_DIM-1)+KERNEL_DIM)/2;
	int countWait = 0;
	int sentPixels = 0;

	uint_8_side_channel dataOutSideChannel;
	uint_8_side_channel currPixelSideChannel;

	if(sel < 4){
		for(int idxPixel = 0; idxPixel < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS);idxPixel++){
			#pragma HLS PIPELINE
			currPixelSideChannel = inStream.read();
			ap_uint<8> data1 = currPixelSideChannel.data;

			currPixelSideChannel = inStream.read();
			ap_uint<8> data2 = currPixelSideChannel.data;

			unsigned char red = (255*data1.range(7,3))/31;
			unsigned char green = (255*( data1.range(2,0)*8 + data2.range(7,5)))/63;
			unsigned char blue = (255*data2.range(4,0))/31;


			unsigned char pixelIn = (red+green+blue)/3;

			lineBuff.shift_up(idxCol);
			lineBuff.insert_top(pixelIn,idxCol);

			for(int idxWinRow = 0; idxWinRow < KERNEL_DIM; idxWinRow++){
				for(int idxWinCol = 0; idxWinCol < KERNEL_DIM; idxWinCol++){
					short val = (short)lineBuff.getval(idxWinRow,idxWinCol+pixConvolved);

					val = (short)kernel[(idxWinRow*KERNEL_DIM)+idxWinCol]*val;
					window.insert(val,idxWinRow,idxWinCol);

				}
			}

			short valOutput = 0;
			if((idxRow >= KERNEL_DIM-1)&&(idxCol >=KERNEL_DIM-1)){
					valOutput = sum(&window);
					if(valOutput <0)
						valOutput = 0;
					pixConvolved++;
			}


			if(idxCol < IMG_WIDTH_OR_COLS -1){
				idxCol++;
			}else{
				idxCol = 0;
				idxRow++;
				pixConvolved = 0;
			}

			countWait++;
			if(countWait > waitTicks){
				valOutput = (31*valOutput)/255;
				dataOutSideChannel.data = (valOutput*8)+(valOutput/4);
				dataOutSideChannel.last = 0;

				outStream.write(dataOutSideChannel);


				dataOutSideChannel.data = (valOutput*64)+(valOutput);
				dataOutSideChannel.last = 0;

				outStream.write(dataOutSideChannel);

				sentPixels++;
			}

		}


		for(countWait = 0; countWait < waitTicks; countWait++){
			dataOutSideChannel.data = 0;
			if(countWait < waitTicks - 1)
				dataOutSideChannel.last = 0;
			else
				dataOutSideChannel.last = 1;

			outStream.write(dataOutSideChannel);
			outStream.write(dataOutSideChannel);
		}
	} else{
		for(int idxPixel = 0; idxPixel < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS);idxPixel++){
			#pragma HLS PIPELINE
			currPixelSideChannel = inStream.read();
			ap_uint<8> data1 = currPixelSideChannel.data;

			currPixelSideChannel = inStream.read();
			ap_uint<8> data2 = currPixelSideChannel.data;

			if(sel == 5){

				unsigned char red = (data1.range(7,3)*255)/31;
				unsigned char green = (255*( data1.range(2,0)*8 + data2.range(7,5)))/63;
				unsigned char blue = (data2.range(4,0)*255)/31;

				unsigned char hue = 0;
				unsigned char saturation = 0;
				unsigned char value = 0;

				unsigned char red_inter = 0;
				unsigned char green_inter = 0;
				unsigned char blue_inter = 0;
				// MAX, MIN among RED, GREEN and BLUE
				int max_val = 0;
				if (red > green) {
					if (red > blue) max_val = red;
					else max_val = blue;
				}
				else{
					if(green>blue) max_val = green;
					else max_val = blue;
				}

				int min_val = 0;
				if (red < green) {
									if (red < blue) max_val = red;
									else min_val = blue;
								}
								else{
									if(green<blue) max_val = green;
									else min_val = blue;
								}
				//(red < green) ? ((red < blue) ? red : blue) : ((green < blue) ? green : blue);

				// Calculate VALUE
				value = max_val;

				if (value>VAL_HIGH||value<VAL_LOW) {
					red_inter = 0;
					blue_inter = 0;
					green_inter = 0;
				}
				else {
					red_inter = 255;
					blue_inter = 255;
					green_inter = 255;
				}



				int delta = 0;

				// Calculate SATURATION
				if (max_val == 0) {
					saturation = 0;
				} else {
					delta = max_val - min_val;
					saturation = (delta * 255) / max_val;
					if (saturation>SAT_HIGH||saturation<SAT_LOW) {
						red_inter = 0;
						blue_inter = 0;
						green_inter = 0;
					}//saturation = 0;
					else{
						red_inter = 255;
						blue_inter = 255;
						green_inter = 255;
					} //saturation = 255;
				}

				// Calculate HUE
				if (delta == 0) {
					hue = 0;
				}
				else if (max_val == red) {
					if (green > blue) {
						hue = (((green - blue) * 60) / delta) % 360;
						if (hue>HUE_HIGH||hue<HUE_LOW) {
							red_inter = 0;
							blue_inter = 0;
							green_inter = 0;
						}//hue = 0;
						else {
							red_inter = 255;
							blue_inter = 255;
							green_inter = 255;
						}//hue = 179;
					}
					else if (blue > green) {
						hue = 360 - (((blue - green) * 60) / delta);
						//if (hue>157||hue<110) hue = 0; else hue = 179;
						if (hue>HUE_HIGH||hue<HUE_LOW) {
													red_inter = 0;
													blue_inter = 0;
													green_inter = 0;
												}//hue = 0;
												else {
													red_inter = 255;
													blue_inter = 255;
													green_inter = 255;
												}//
					}
				}
				else if (max_val == green) {
					if (blue > red) {
						hue = (((blue - red) * 60) / delta) + 120;
						//if (hue>157||hue<110) hue = 0; else hue = 179;
						if (hue>HUE_HIGH||hue<HUE_LOW) {
													red_inter = 0;
													blue_inter = 0;
													green_inter = 0;
												}//hue = 0;
												else {
													red_inter = 255;
													blue_inter = 255;
													green_inter = 255;
												}//
					}
					else if (red > blue) {
						hue = 120 - (((red - blue) * 60) / delta);
						//if (hue>157||hue<110) hue = 0; else hue = 179;
						if (hue>HUE_HIGH||hue<HUE_LOW) {
													red_inter = 0;
													blue_inter = 0;
													green_inter = 0;
												}//hue = 0;
												else {
													red_inter = 255;
													blue_inter = 255;
													green_inter = 255;
												}//
					}
				}
				else if (max_val == blue) {
					if (red > green) {
						hue = (((red - green) * 60) / delta) + 240;
						//if (hue>157||hue<110) hue = 0; else hue = 179;
						if (hue>HUE_HIGH||hue<HUE_LOW) {
													red_inter = 0;
													blue_inter = 0;
													green_inter = 0;
												}//hue = 0;
												else {
													red_inter = 255;
													blue_inter = 255;
													green_inter = 255;
												}//
					}
					else if (green > red) {
						hue = 240 - (((green - red) * 60) / delta);
						//if (hue>157||hue<110) hue = 0; else hue = 179;
						if (hue>HUE_HIGH||hue<HUE_LOW) {
													red_inter = 0;
													blue_inter = 0;
													green_inter = 0;
												}//hue = 0;
												else {
													red_inter = 255;
													blue_inter = 255;
													green_inter = 255;
												}//
					}
				}

				/*
				bool thr = (red < 32) & (green < 32) & (blue > 15);

				if(thr == false){
					red = 0;
					green = 0;
					blue = 0;
				}
				*/


				dataOutSideChannel.data = (31*(red_inter/255)*8)+(31*(green_inter/255)/8);
				dataOutSideChannel.last = 0;
				//dataOutSideChannel.data = hue*8+ saturation/8;
				//dataOutSideChannel.last = 0;

				outStream.write(dataOutSideChannel);

				dataOutSideChannel.data = (31*(green_inter/255)*64) + (31*blue_inter)/255;
				dataOutSideChannel.last = 0;

				outStream.write(dataOutSideChannel);
			}else{
				dataOutSideChannel.data = data1;
				dataOutSideChannel.last = 0;

				outStream.write(dataOutSideChannel);

				dataOutSideChannel.data = data2;
				dataOutSideChannel.last = 0;

				outStream.write(dataOutSideChannel);
			}

		}
	}

}

short sum(hls::Window<KERNEL_DIM,KERNEL_DIM,short> *window){
		short accumulator = 0;

		for(int idxRow = 0; idxRow < KERNEL_DIM; idxRow++){
			for(int idxCol = 0; idxCol < KERNEL_DIM; idxCol++){
				accumulator = accumulator + (short)window->getval(idxRow,idxCol);
			}
		}
		return accumulator;
}
