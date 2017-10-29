#include "iobmp.h"

// Reverse Data Conversion ========================================================================


void bmpFile::convertPixel2Data_BW() {
	for(int i = 0; i < pixelArray.size(); i++) {
		int r = pixelArray[i].rgbRed;
		int g = pixelArray[i].rgbGreen;
		int b = pixelArray[i].rgbBlue;

		double bw = (std::min(r, std::min(g, b)) + std::max(r, std::max(g, b))) * 0.5;

		pixelData.push_back(bw);
	}

}

void bmpFile::convertPixel2Data_AVG() {
	for(int i = 0; i < pixelArray.size(); i++) {
		double avg = pixelArray[i].rgbRed + pixelArray[i].rgbGreen + pixelArray[i].rgbBlue;
		avg /= 3;
		pixelData.push_back(avg);
	}
}

void bmpFile::convertPixel2Data_RGB() {
	for(int i = 0; i < pixelArray.size(); i++) {
		int testVal = (i + 1) % 3;
		if(testVal == 2) {
			pixelData.push_back(pixelArray[i].rgbRed);
		}
		if(testVal == 1) {
			pixelData.push_back(pixelArray[i].rgbGreen);
		}
		if(testVal == 0) {
			pixelData.push_back(pixelArray[i].rgbBlue);
		}
	}
}
