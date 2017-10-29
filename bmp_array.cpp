#include "iobmp.h"

// Pixel Array Mutator Functions ==================================================================

// Write Pixel Array from Data Array Data
// Parameters: color interpretation specifier, color scale factor
// Preconditions: data array contains some valid value, color is a value between 0 and 2
// Postconditions: pixel array contains data based on a conversion of the data array
void bmpFile::writePixelArray(int color, int scalefac  = 0) {

	erasePixelArray();

	for(int i = 0; i < pixelData.size(); i++) {
		int temp = pixelData[i]; 		// temp data value
		RGBQUAD tempPixel; 				// temp Pixel
		if(color == 0) { 				// 2-bit color
			if(temp > scalefac) {
				tempPixel.rgbBlue = 255;
				tempPixel.rgbGreen = 255;
				tempPixel.rgbRed = 255;
				tempPixel.rgbReserved = 0;
			}
			if(temp <= scalefac) {
				tempPixel.rgbBlue = 0;
				tempPixel.rgbGreen = 0;
				tempPixel.rgbRed = 0;
				tempPixel.rgbReserved = 0;
			}
		}
		if(color == 1) { 				// grey scale interpolation
			convertGreyScale(temp, scalefac, tempPixel);
		}
		if(color == 2) { 				// color gradient interpolation
			convertColorGradient(temp, scalefac, tempPixel);
		}
		/* if(color == 3) { // color index representation
			this->convertColorIndex(temp, tempPixel);
		} */

		pixelArray.push_back(tempPixel);
	}
	return;
}

// Write Pixel Point Function
// Parameters: a unsigned char value (0 - 256) for r, g, b
// Preconditions: BMP object is initialized
// Postconditions: RGBQUAD with r,g,b is added to end of Pixel Array.
void bmpFile::writePixelPoint(BYTE r, BYTE g, BYTE b) {
	// this function manually writes a pixel

	RGBQUAD tempPixel;
	tempPixel.rgbRed = r;
	tempPixel.rgbGreen = g;
	tempPixel.rgbBlue = b;

	pixelArray.push_back(tempPixel);

	return;
}

// Erase last Pixel Function
void bmpFile::erasePixelLast() {
	pixelArray.pop_back();
	return;
}

// Erase Pixel Array
void bmpFile::erasePixelArray() {
	pixelArray.clear();
	return;
}

// Data Array Mutator Functions ===================================================================

// Write Data Point Function
void bmpFile::writeDataPoint(int data) {
	pixelData.push_back(data);
	return;
}

// Erase Data Point Function
void bmpFile::eraseDataLast() {
	pixelData.pop_back();
	return;
}

// Erase Data Array Function
void bmpFile::eraseDataArray() {
	pixelData.clear();
	return;
}
