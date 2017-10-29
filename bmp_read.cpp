#include "iobmp.h"

// Read Data from Object ==========================================================================

unsigned int bmpFile::bmpSizeIs_x() const {
	return image_x;
}

unsigned int bmpFile::bmpSizeIs_y() const {
	return image_y;
}

unsigned int bmpFile::bmpDpiIs_x() const {
	return dpi_x;
}

unsigned int bmpFile::bmpDpiIs_y() const {
	return dpi_y;
}

int bmpFile::readDataPoint(int position) const {
	// this function reads a single data point from the pixel data array, whose address is

	int length = this->vectorDataLength();
	if(length >= position) {
		return pixelData[position];
	} else {
		return 0;
	}
}

unsigned int bmpFile::readPixel_Red(int position) const {
	// this function reads from a single pixel, and returns the red value, based on the address passed

	int length = this->vectorPixelLength();
	if(length >= position) {
		return pixelArray[position].rgbRed;
	} else {
		return 0;
	}
}

unsigned int bmpFile::readPixel_Green(int position) const {
	// this function reads from a single pixel, and returns the red value, based on the address passed

	int length = this->vectorPixelLength();
	if(length >= position) {
		return pixelArray[position].rgbGreen;
	} else {
		return 0;
	}
}

unsigned int bmpFile::readPixel_Blue(int position) const {
	// this function reads from a single pixel, and returns the red value, based on the address passed

	int length = this->vectorPixelLength();
	if(length >= position) {
		return pixelArray[position].rgbBlue;
	} else {
		return 0;
	}
}

int bmpFile::vectorDataLength() const {
	// this function returns the length of the pixel data vector

	int temp = pixelData.size();
	return temp;
}

int bmpFile::vectorPixelLength() const {
	// this function returns the length of the pixel array vector

	int temp = pixelArray.size();
	return temp;
}
