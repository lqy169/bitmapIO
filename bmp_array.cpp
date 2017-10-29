#include "iobmp.h"

// Write Data to Object Functions =================================================================

void bmpFile::writePixelArray(int color, int scalefac) {
	// this function converts the data array to color pixel array, depending on color mode

	if(DEBUG_LEVEL >= 1) { std::cout << "#Writing Pixel Array::" << std::endl; }
	this->erasePixelArray();

	for(int i = 0; i < pixelData.size(); i++) {
		if(DEBUG_LEVEL >= 2) { std::cout << "#Pixel " << i << "::" << std::endl; }
		int temp = pixelData[i]; // temp data value
		RGBQUAD tempPixel; // temp Pixel
		if(color == 0) { // 2-bit color
			if(temp > 0) {
				tempPixel.rgbBlue = 255;
				tempPixel.rgbGreen = 255;
				tempPixel.rgbRed = 255;
				tempPixel.rgbReserved = 0;
			}
			if(temp <= 0) {
				tempPixel.rgbBlue = 0;
				tempPixel.rgbGreen = 0;
				tempPixel.rgbRed = 0;
				tempPixel.rgbReserved = 0;
			}
		}
		if(color == 1) { // grey scale representation
			this->convertGreyScale(temp, scalefac, tempPixel);
		}
		if(color == 2) { // color gradient representation
			this->convertColorGradient(temp, scalefac, tempPixel);
		}
		/* if(color == 3) { // color index representation
			this->convertColorIndex(temp, tempPixel);
		} */
		pixelArray.push_back(tempPixel);
	}
	return;
}

void bmpFile::writePixelPoint(char r, char g, char b) {
	// this function manually writes a pixel

	RGBQUAD tempPixel;
	tempPixel.rgbRed = r;
	tempPixel.rgbGreen = g;
	tempPixel.rgbBlue = b;

	if(DEBUG_LEVEL >= 1) { std::cout << "#Pixel Written::" << std::endl; }
	pixelArray.push_back(tempPixel);

	return;
}

void bmpFile::erasePixelArray() {
	// this function erases the pixel color array

	pixelArray.clear();
	if(DEBUG_LEVEL >= 2) { std::cout << "#Pixel Array Erased::" << std::endl; }
	return;
}

void bmpFile::writeDataPoint(int data) {
	// this function writes a data value to a pixel

	pixelData.push_back(data);
	if(DEBUG_LEVEL >= 2) { std::cout << "#Data Point Written::" << std::endl; }
	return;
}

void bmpFile::eraseDataLast() {
	// this function erases the last data point

	pixelData.pop_back();
	if(DEBUG_LEVEL >= 2) { std::cout << "#Last Data Point Erased::" << std::endl; }
	return;
}

void bmpFile::eraseDataArray() {
	// this function erases the pixel data array

	pixelData.clear();
	if(DEBUG_LEVEL >= 2) { std::cout << "#Data Array Erased::" << std::endl; }
	return;
}
