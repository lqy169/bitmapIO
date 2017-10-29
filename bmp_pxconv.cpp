#include "iobmp.h"

// Convert Data to Pixel Array ===============================================================

void bmpFile::convertGreyScale(int temp, int scalefac, RGBQUAD &tempPixel) {
	// this function converts the data point to a greyscale value

	int tempScale;
	tempScale = (temp * scalefac) % 512; // 128 + 128 to include negatives
	int tempScaleInv = (tempScale - 768) * -1;

	if(tempScale < 256) {
		tempPixel.rgbBlue = tempScale;
		tempPixel.rgbGreen = tempScale;
		tempPixel.rgbRed = tempScale;
	} else {
		tempPixel.rgbBlue = tempScaleInv;
		tempPixel.rgbGreen = tempScaleInv;
		tempPixel.rgbRed = tempScaleInv;
	}

	tempPixel.rgbReserved  = 0;
}

void bmpFile::convertColorGradient(int temp, int scalefac, RGBQUAD &tempPixel) {
	// this function converts the data point to a color value, based on a rotating RGB spectrum

	// RGB Color
	double value = 0;
	double valuecut = 256.0 / static_cast<double>(scalefac);
	if(temp < valuecut) {
		value = static_cast<double>(temp) / valuecut;
	} else if(temp >= valuecut) {
		value = 1;
	}

	if(value > 1) { value = 1; }

	temp = (temp * scalefac) % 1536;

	tempPixel.rgbRed = 0;
	tempPixel.rgbGreen = 0;
	tempPixel.rgbBlue = 0;

	// Red
	if(256 >= temp || temp >= 1280) {
		tempPixel.rgbRed = 255;
	} else if(512 < temp && temp < 1024) {
		tempPixel.rgbRed = 0;
	} else if(1024 <= temp && temp < 1280) {
		tempPixel.rgbRed = temp - 1024;
	} else if(256 < temp && temp <= 512) {
		tempPixel.rgbRed = ((temp - 256) - 256) * -1;
	}

	// Green
	if(256 <= temp && temp <= 768) {
		tempPixel.rgbGreen = 255;
	} else if(temp > 1024) {
		tempPixel.rgbGreen = 0;
	} else if(temp < 256) {
		tempPixel.rgbGreen = temp;
	} else if(768 < temp && temp <= 1024) {
		tempPixel.rgbGreen = ((temp - 768) - 256) * -1;
	}

	// Blue
	if(768 <= temp && temp <= 1280) {
		tempPixel.rgbBlue = 255;
	} else if(temp < 512) {
		tempPixel.rgbBlue = 0;
	} else if(512 <= temp && temp < 768) {
		tempPixel.rgbBlue = temp - 512;
	} else if(1280 < temp && temp <= 1536) {
		tempPixel.rgbBlue = ((temp - 1280) - 256) * -1;
	}

	// Saturation?

	// Value
	tempPixel.rgbRed = tempPixel.rgbRed * value;
	tempPixel.rgbGreen = tempPixel.rgbGreen * value;
	tempPixel.rgbBlue = tempPixel.rgbBlue * value;

	/* HSV Color - Broken
	double sat = 1;
	double val = 1;

	if(sat == 0) {
		tempPixel.rgbRed = tempPixel.rgbGreen = tempPixel.rgbBlue = val;
	}

	double hue;
	temp = temp * scalefac;
	if((temp % 720) < 360) {
		hue = static_cast<double>(temp % 360) / 60;
	} else {
		hue = static_cast<double>((temp % 720 - 720) * (-1)) / 60;
	}

	int i = floor(hue);
	double f = hue - i;

	double p = val * (1 - sat);
	double q = val * (1 - sat * f);
	double t = val * (1 - sat * (1 - f));

	val *= 256;
	p *= 256;
	q *= 256;
	t *= 256;

	switch(i) {
	case 0:
		tempPixel.rgbRed = val;
		tempPixel.rgbGreen = t;
		tempPixel.rgbBlue = p;
		break;
	case 1:
		tempPixel.rgbRed = q;
		tempPixel.rgbGreen = val;
		tempPixel.rgbBlue = p;
		break;
	case 2:
		tempPixel.rgbRed = p;
		tempPixel.rgbGreen = val;
		tempPixel.rgbBlue = t;
		break;
	case 3:
		tempPixel.rgbRed = p;
		tempPixel.rgbGreen = q;
		tempPixel.rgbBlue = val;
		break;
	case 4:
		tempPixel.rgbRed = t;
		tempPixel.rgbGreen = p;
		tempPixel.rgbBlue = val;
		break;
	default:
		tempPixel.rgbRed = val;
		tempPixel.rgbGreen = p;
		tempPixel.rgbBlue = q;
		break;
	}

	*/

	tempPixel.rgbReserved = 0;

	return;
}
