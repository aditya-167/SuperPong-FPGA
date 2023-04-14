#include "imgfiltering.h"
#include "stdio.h"

#define HUE_HIGH 157
#define HUE_LOW	 110

#define SAT_HIGH 255
#define SAT_LOW  047

#define	VAL_HIGH 111
#define VAL_LOW  255


void img_filter(hls::stream<uint_8_side_channel>& inStream, hls::stream<uint_8_side_channel>& outStream, ap_uint<10>* coord) {
	#pragma HLS INTERFACE axis port=inStream
	#pragma HLS INTERFACE axis port=outStream

	uint_8_side_channel dataOutSideChannel;
	uint_8_side_channel currPixelSideChannel;

	ap_uint<1> col_sum[IMG_WIDTH_OR_COLS];

	ap_uint<10> start_coord = 0;
	ap_uint<10> end_coord = 0;

	for (int idxPixel = 0; idxPixel < (IMG_WIDTH_OR_COLS * IMG_HEIGHT_OR_ROWS); idxPixel++) {
		#pragma HLS PIPELINE
		currPixelSideChannel = inStream.read();
		ap_uint<8> data1 = currPixelSideChannel.data;

		currPixelSideChannel = inStream.read();
		ap_uint<8> data2 = currPixelSideChannel.data;

		unsigned char red = (data1.range(7, 3) * 255) / 31;
		unsigned char green = (255 * (data1.range(2, 0) * 8 + data2.range(7, 5))) / 63;
		unsigned char blue = (data2.range(4, 0) * 255) / 31;

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
		else {
			if (green > blue) max_val = green;
			else max_val = blue;
		}

		int min_val = 0;
		if (red < green) {
			if (red < blue) max_val = red;
			else min_val = blue;
		}
		else {
			if (green < blue) max_val = green;
			else min_val = blue;
		}

		// Calculate VALUE
		value = max_val;
		if (value > VAL_HIGH || value < VAL_LOW) {
			red_inter = 0; blue_inter = 0; green_inter = 0;
		}
		else {
			red_inter = 255; blue_inter = 255; green_inter = 255;
		}

		int delta = 0;

		// Calculate SATURATION
		if (max_val == 0)
			saturation = 0;
		else {
			delta = max_val - min_val;
			saturation = (delta * 255) / max_val;

			if (saturation > SAT_HIGH || saturation < SAT_LOW) {
				red_inter = 0; blue_inter = 0; green_inter = 0;
			}
			else {
				red_inter = 255; blue_inter = 255; green_inter = 255;
			}
		}

		// Calculate HUE
		if (delta == 0)
			hue = 0;
		else if (max_val == red) {
			if (green > blue) {
				hue = (((green - blue) * 60) / delta) % 360;
				if (hue > HUE_HIGH || hue < HUE_LOW) {
					red_inter = 0; blue_inter = 0; green_inter = 0;
				}
				else {
					red_inter = 255; blue_inter = 255; green_inter = 255;
				}
			}
			else if (blue > green) {
				hue = 360 - (((blue - green) * 60) / delta);
				if (hue > HUE_HIGH || hue < HUE_LOW) {
					red_inter = 0; blue_inter = 0; green_inter = 0;
				}
				else {
					red_inter = 255; blue_inter = 255; green_inter = 255;
				}
			}
		}
		else if (max_val == green) {
			if (blue > red) {
				hue = (((blue - red) * 60) / delta) + 120;
				if (hue > HUE_HIGH || hue < HUE_LOW) {
					red_inter = 0; blue_inter = 0; green_inter = 0;
				}
				else {
					red_inter = 255; blue_inter = 255; green_inter = 255;
				}
			}
			else if (red > blue) {
				hue = 120 - (((red - blue) * 60) / delta);

				if (hue > HUE_HIGH || hue < HUE_LOW) {
					red_inter = 0; blue_inter = 0; green_inter = 0;
				}
				else {
					red_inter = 255; blue_inter = 255; green_inter = 255;
				}
			}
		}
		else if (max_val == blue) {
			if (red > green) {
				hue = (((red - green) * 60) / delta) + 240;
				if (hue > HUE_HIGH || hue < HUE_LOW) {
					red_inter = 0; blue_inter = 0; green_inter = 0;
				}
				else {
					red_inter = 255; blue_inter = 255; green_inter = 255;
				}
			}
			else if (green > red) {
				hue = 240 - (((green - red) * 60) / delta);
				if (hue > HUE_HIGH || hue < HUE_LOW) {
					red_inter = 0; blue_inter = 0; green_inter = 0;
				}
				else {
					red_inter = 255; blue_inter = 255; green_inter = 255;
				}
			}
		}

		if (red_inter + green_inter + blue_inter > 0)
			col_sum[idxPixel % IMG_WIDTH_OR_COLS] = 1;
		else
			col_sum[idxPixel % IMG_WIDTH_OR_COLS] = 0;

		dataOutSideChannel.data = (31 * (red_inter / 255) * 8) + (31 * (green_inter / 255) / 8);
		dataOutSideChannel.last = 0;

		outStream.write(dataOutSideChannel);

		dataOutSideChannel.data = (31 * (green_inter / 255) * 64) + (31 * blue_inter) / 255;
		dataOutSideChannel.last = 0;

		outStream.write(dataOutSideChannel);
	}

	for (int col = 0; col < IMG_WIDTH_OR_COLS; col++) {
		if (col_sum[col] > 0) {
			start_coord = col;
			break;
		}
	}

	for (int col = IMG_WIDTH_OR_COLS - 1; col >= 0; col--) {
		if (col_sum[col] > 0) {
			end_coord = col;
			break;
		}
	}

	*coord = start_coord + ((start_coord - end_coord)/2);

}