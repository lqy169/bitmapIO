#include "iobmp.h"

// Read Data from Object ==========================================================================

unsigned int bmpFile::bmpSizeIs_x() const {
	if(DEBUG_LEVEL == 2) { std::cout << "#Reading image x::" << std::endl; }
	return image_x;
}

unsigned int bmpFile::bmpSizeIs_y() const {
	if(DEBUG_LEVEL == 2) { std::cout << "#Reading image y::" << std::endl; }
	return image_y;
}

unsigned int bmpFile::bmpDpiIs_x() const {
	if(DEBUG_LEVEL == 2) { std::cout << "#Reading dpi x::" << std::endl; }
	return dpi_x;
}

unsigned int bmpFile::bmpDpiIs_y() const {
	if(DEBUG_LEVEL == 2) { std::cout << "#Reading dpi y::" << std::endl; }
	return dpi_y;
}

int bmpFile::readDataPoint(int position) const {
	// this function reads a single data point from the pixel data array, whose address is 

	if(DEBUG_LEVEL == 2) { std::cout << "#Reading Data at pos: " << position << "::" << std::endl; }

	int length = this->vectorDataLength();
	if(length >= position) {
		return pixelData[position];
	} else {
		if(length < position && DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Data Array does not include your pos: " << position << "::" << std::endl; }
		else if(length == 0 && DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Data Array does not include any values::" << std::endl; }
		else if(DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Unknown Error::" << std::endl; }
		return 0;
	}
}

unsigned int bmpFile::readPixel_Red(int position) const {
	// this function reads from a single pixel, and returns the red value, based on the address passed

	if(DEBUG_LEVEL == 2) { std::cout << "#Reading Red at pos: " << position << "::" << std::endl; }

	int length = this->vectorPixelLength();
	if(length >= position) {
		return pixelArray[position].rgbRed;
	} else {
		if(length < position && DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Data Array does not include your pos: " << position << "::" << std::endl; }
		else if(length == 0 && DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Data Array does not include any values::" << std::endl; }
		else if(DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Unknown Error::" << std::endl; }
		return 0;
	}
}

unsigned int bmpFile::readPixel_Green(int position) const {
	// this function reads from a single pixel, and returns the red value, based on the address passed

	if(DEBUG_LEVEL == 2) { std::cout << "#Reading Green at pos: " << position << "::" << std::endl; }

	int length = this->vectorPixelLength();
	if(length >= position) {
		return pixelArray[position].rgbGreen;
	} else {
		if(length < position && DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Data Array does not include your pos: " << position << "::" << std::endl; }
		else if(length == 0 && DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Data Array does not include any values::" << std::endl; }
		else if(DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Unknown Error::" << std::endl; }
		return 0;
	}
}

unsigned int bmpFile::readPixel_Blue(int position) const {
	// this function reads from a single pixel, and returns the red value, based on the address passed

	if(DEBUG_LEVEL == 2) { std::cout << "#Reading Blue at pos: " << position << "::" << std::endl; }

	int length = this->vectorPixelLength();
	if(length >= position) {
		return pixelArray[position].rgbBlue;
	} else {
		if(length < position && DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Data Array does not include your pos: " << position << "::" << std::endl; }
		else if(length == 0 && DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Data Array does not include any values::" << std::endl; }
		else if(DEBUG_LEVEL == 1) { std::cout << "#ERROR!!Unknown Error::" << std::endl; }
		return 0;
	}
}

int bmpFile::vectorDataLength() const {
	// this function returns the length of the pixel data vector

	int temp = pixelData.size();
	if(DEBUG_LEVEL >= 1) { std::cout << "#Data Vector Length is " << temp << "::" << std::endl; }
	return temp;
}

int bmpFile::vectorPixelLength() const {
	// this function returns the length of the pixel array vector

	int temp = pixelArray.size();
	if(DEBUG_LEVEL >= 1) { std::cout << "#Pixel Vector Length is " << temp << "::" << std::endl; }
	return temp;
}
