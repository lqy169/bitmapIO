// Include Guards =================================================================================
#ifndef __IOBMP_H_INCLUDED__
#define __IOBMP_H_INCLUDED__

// Forward Declared ===============================================================================

class bmpFile;
typedef unsigned char BYTE;      // 1 byte
typedef unsigned short int WORD; // 2 bytes
typedef unsigned int DWORD;      // 4 bytes

int DEBUG_LEVEL; // should be defined in main

// Dependencies ==================================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

// Class and Structure Declarations ===============================================================

struct bmpHeader { // Defines Bitmap header structure
	WORD bfType; // must be set to 'B' 'F', or 19778
	DWORD bfSize; // the length of the entire bitmap file in bytes
	WORD bfReserved1; // must be set to 0
	WORD bfReserved2; // ---------- ditto
	DWORD bfOffBits; // Offset from this point to pixel Data, basically length of bmpInfoHeader
};

struct bmpInfoHeader { // Defines Bitmap Info header structure
	DWORD biSize; // Size of bmpInfoHeader structure
	DWORD biWidth; // width of the final image
	DWORD biHeight; // height of the final image
	WORD biPlanes; // must be set to 1
	WORD biBitCount; // number of bits per pixel, this library currently only supports 24 bit
	DWORD biCompression; // type of compression, currently doesn't work
	DWORD biSizeImage; // Size of pixel data
	DWORD biXPelsPerMeter; // basically dpi on x
	DWORD biYPelsPerMeter; // basically dpi on y
	DWORD biClrUsed; // number of colors used, 0 uses all in pixel data
	DWORD biClrImportant; // number of important colors, 0 uses all
};

struct RGBQUAD { // Pixel structure, used for both pixel array and color table
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved; // must be set to 0
};


// bmp Image file class ===========================================================================
class bmpFile { 
public:
	// constructor
	bmpFile();

	// open object functions
	void openNew(int x, int y, int dpi, std::string file);		// | Opens a new object file
	void openLoad(std::string file);				// | Loads a pre-existing file
	void changeFile(std::string newFile); 				// | Change the filename
	std::string fileNameIs();					// | Read filename from object

	void loadColorIndex(std::string file);				// | Load color index table from file == DOSEN'T EXIST YET

	// write data to object
	// - Pixel Array
	void writePixelArray(int color, int scalefac);			// | Write the pixel array out using data vector
	void writePixelPoint(char r, char g, char b);			// | Manually write a pixel
	void erasePixelArray();						// | Erase pixel array

	// - Data Array
	void writeDataPoint(int data);					// | Write a point of Data
	void eraseDataLast();						// | Erase last data point
	void eraseDataArray();						// | Erase entire data array

	// read data from object
	// - Member variable read
	unsigned int bmpSizeIs_x() const;				// | bmp X dimension
	unsigned int bmpSizeIs_y() const;				// | bmp Y dimension
	unsigned int bmpDpiIs_x() const;				// | bmp X dpi
	unsigned int bmpDpiIs_y() const;				// | bmp Y dpi

	// - Data/Pixel array read
	int readDataPoint(int position) const;				// | Read point from data vector
	unsigned int readPixel_Red(int position) const;			// | Read red pixel from pixel vector
	unsigned int readPixel_Green(int position) const;		// | Read green pixel from pixel vector
	unsigned int readPixel_Blue(int position) const;		// | Read blue pixel from pixel vector

	int vectorDataLength() const;					// | Return length of Data array
	int vectorPixelLength() const;					// | Return length of Pixel Array

	// Reverse Data conversion
	void convertPixel2Data_BW();					// | Turns Pixel data to BW data array using Luminosity focus
	void convertPixel2Data_AVG();					// | Turns Pixel data to BW data array using Average focus
	void convertPixel2Data_RGB();					// | Turns Pixel data to BW data array, keeping Red, Green, and Blue

	// Finalize and output bitmap object
	void BMPfinalize(std::string file);				// | Finalizes object and writes

private:
	// Member Variables
	std::string fileName;
	bmpHeader header;
	bmpInfoHeader dibheader;

	unsigned int image_x = 0;
	unsigned int image_y = 0;
	unsigned int dpi_x = 0;
	unsigned int dpi_y = 0;

	std::vector<int> pixelData;					// | unformated data, x l 2 r, y b 2 t
	std::vector<RGBQUAD> pixelArray; 				// | formatted in rgb, x l 2 r, y b 2 t

	// Data to Color Conversion functions
	void convertGreyScale(int temp, int scalefac, RGBQUAD &tempPixel);	// Converts Data to greyscale color
	void convertColorGradient(int temp, int scalefac, RGBQUAD &tempPixel);	// Converts Data to color gradient
	void convertColorIndex(int temp, RGBQUAD &tempPixel);			// Converts Data to color based on index table

	// Output Function
	void writeFile();						// | Write file function, used by finalize
};

// BMP Class Constructor ==========================================================================

bmpFile::bmpFile() {
	header.bfType = 19778;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = 54;

	dibheader.biSize = 40;

	dibheader.biCompression = 0;

	dibheader.biPlanes = 1;
	dibheader.biBitCount = 24; // Color depth of image

	dibheader.biClrUsed = 0;
	dibheader.biClrImportant = 0;
}

// Open Object Functions ==========================================================================

void bmpFile::openNew(int x, int y, int dpi, std::string file) {
	// this function sets resolution variables for a new image object.
	
	image_x = x;
	dibheader.biWidth = image_x;
	image_y = y;
	dibheader.biHeight = image_y;

	dpi_x = dpi * 39.3701;
	dibheader.biXPelsPerMeter = dpi_x;
	dpi_y = dpi * 39.3701;
	dibheader.biYPelsPerMeter = dpi_y;

	fileName = file;

	return;
}

void bmpFile::openLoad(std::string file) {
	// this function will load a image file to a new image object.
	if(DEBUG_LEVEL >= 1) { std::cout << "#Opening File Stream::" << std::endl; }

	std::ifstream input(file, std::ios::in|std::ios::binary);
	this->erasePixelArray();
	this->eraseDataArray();

	if (input.fail()) {
	    std::cout << "#ERROR!! Cannot open the file::" << std::endl;
	    return;
	}

	//Reading BMP elements
	input.read(reinterpret_cast<char*>(&header.bfType), sizeof(header.bfType));
	input.read(reinterpret_cast<char*>(&header.bfSize), sizeof(header.bfSize));
	input.read(reinterpret_cast<char*>(&header.bfReserved1), sizeof(header.bfReserved1));
	input.read(reinterpret_cast<char*>(&header.bfReserved2), sizeof(header.bfReserved2));
	input.read(reinterpret_cast<char*>(&header.bfOffBits), sizeof(header.bfOffBits));

	input.read(reinterpret_cast<char*>(&dibheader.biSize), sizeof(dibheader.biSize));
	input.read(reinterpret_cast<char*>(&dibheader.biWidth), sizeof(dibheader.biWidth));
	input.read(reinterpret_cast<char*>(&dibheader.biHeight), sizeof(dibheader.biHeight));
	input.read(reinterpret_cast<char*>(&dibheader.biPlanes), sizeof(dibheader.biPlanes));
	input.read(reinterpret_cast<char*>(&dibheader.biBitCount), sizeof(dibheader.biBitCount));
	input.read(reinterpret_cast<char*>(&dibheader.biCompression), sizeof(dibheader.biCompression));
	input.read(reinterpret_cast<char*>(&dibheader.biSizeImage), sizeof(dibheader.biSizeImage));
	input.read(reinterpret_cast<char*>(&dibheader.biXPelsPerMeter), sizeof(dibheader.biXPelsPerMeter));
	input.read(reinterpret_cast<char*>(&dibheader.biYPelsPerMeter), sizeof(dibheader.biYPelsPerMeter));
	input.read(reinterpret_cast<char*>(&dibheader.biClrUsed), sizeof(dibheader.biClrUsed));
	input.read(reinterpret_cast<char*>(&dibheader.biClrImportant), sizeof(dibheader.biClrImportant));

	image_x = dibheader.biWidth;
	image_y = dibheader.biHeight;

	dpi_x = static_cast<double>(dibheader.biXPelsPerMeter) / 39.3701;
	dpi_y = static_cast<double>(dibheader.biYPelsPerMeter) / 39.3701;

	fileName = file;

	// Pixel Array Write
	for(int y = 0; y <= image_y; y++) {
		for(int x = 0; x < image_x; x++) {
			if(DEBUG_LEVEL >= 2) { std::cout << "#Reading Pixel " << x << ' ' << y << "::" << std::endl; }
			RGBQUAD tempPixel;
			input.read(reinterpret_cast<char*>(&tempPixel.rgbBlue), 1);
			input.read(reinterpret_cast<char*>(&tempPixel.rgbGreen), 1);
			input.read(reinterpret_cast<char*>(&tempPixel.rgbRed), 1);
			pixelArray.push_back(tempPixel);
		}
		for(int n = 0; n < (image_x % 4); n++) { // to ensure multipe of 4 requirement of bmp data
			int extra;
			input.read(reinterpret_cast<char*>(&extra), 1);
			if(DEBUG_LEVEL >= 2) { std::cout << "#Ignoring Extra::" << std::endl; }
		}
	}

	// Conclusion
	input.close();
	if(DEBUG_LEVEL >= 1) { std::cout << "#Closing Input Stream::" << std::endl; }
	return;
}

void bmpFile::changeFile(std::string newFile) {
	// This function changes the pointed file
	if(DEBUG_LEVEL == 2) { std::cout << "#Changing file path::" << std::endl; }
	fileName = newFile;
	return;
}

std::string bmpFile::fileNameIs() {
	// This function returns the pointed file
	if(DEBUG_LEVEL == 2) { std::cout << "#Reading file path::" << std::endl; }
	return fileName;
}

void bmpFile::convertColorIndex(int temp, RGBQUAD &tempPixel) {
	std::cout << "I don't exist yet!";
	return;
}

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

// Reverse Data Conversion ========================================================================

void bmpFile::convertPixel2Data_BW() {
	for(int i = 0; i < pixelArray.size(); i++) {
		if(DEBUG_LEVEL >= 2) { std::cout << "#Converting pixel " << i << "::" << std::endl; }
		int r = pixelArray[i].rgbRed;
		int g = pixelArray[i].rgbGreen;
		int b = pixelArray[i].rgbBlue;

		double bw = (std::min(r, std::min(g, b)) + std::max(r, std::max(g, b))) * 0.5f;

		pixelData.push_back(bw);
	}
	
}

void bmpFile::convertPixel2Data_AVG() {
	for(int i = 0; i < pixelArray.size(); i++) {
		if(DEBUG_LEVEL >= 2) { std::cout << "#Converting pixel " << i << "::" << std::endl; }
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
		if(DEBUG_LEVEL >= 2) { std::cout << "#Converting pixel " << i << "::" << std::endl; }
	}
}

// Finalize and output BMP object =================================================================

void bmpFile::BMPfinalize(std::string file) {
	// this function should be called to finalize pixel data and write bmpfile

	if(DEBUG_LEVEL >= 1) { std::cout << "#Beginning Finalization::" << std::endl; }

	dibheader.biSizeImage = (image_x * 3 + image_x % 4) * image_y;
	header.bfSize = dibheader.biSizeImage + 54;

	fileName = file;

	writeFile();
	return;
}

// Private Member Functions =======================================================================

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

	if(DEBUG_LEVEL >= 2) { std::cout << "#Processing Color::" << std::endl; }

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

void bmpFile::writeFile() {
	// this function begins the writeout process of the class data

	std::ofstream output;
	if(DEBUG_LEVEL >= 1) { std::cout << "#Opening File::" << std::endl; }
	output.open(fileName, std::ios::out | std::ios::binary);

	//Writing BMP elements
	output.write(reinterpret_cast<char*>(&header.bfType), sizeof(header.bfType));
	output.write(reinterpret_cast<char*>(&header.bfSize), sizeof(header.bfSize)); // calculate
	output.write(reinterpret_cast<char*>(&header.bfReserved1), sizeof(header.bfReserved1));
	output.write(reinterpret_cast<char*>(&header.bfReserved2), sizeof(header.bfReserved2));
	output.write(reinterpret_cast<char*>(&header.bfOffBits), sizeof(header.bfOffBits));

	output.write(reinterpret_cast<char*>(&dibheader.biSize), sizeof(dibheader.biSize));
	output.write(reinterpret_cast<char*>(&dibheader.biWidth), sizeof(dibheader.biWidth));
	output.write(reinterpret_cast<char*>(&dibheader.biHeight), sizeof(dibheader.biHeight));
	output.write(reinterpret_cast<char*>(&dibheader.biPlanes), sizeof(dibheader.biPlanes));
	output.write(reinterpret_cast<char*>(&dibheader.biBitCount), sizeof(dibheader.biBitCount));
	output.write(reinterpret_cast<char*>(&dibheader.biCompression), sizeof(dibheader.biCompression));
	output.write(reinterpret_cast<char*>(&dibheader.biSizeImage), sizeof(dibheader.biSizeImage)); // calculate
	output.write(reinterpret_cast<char*>(&dibheader.biXPelsPerMeter), sizeof(dibheader.biXPelsPerMeter));
	output.write(reinterpret_cast<char*>(&dibheader.biYPelsPerMeter), sizeof(dibheader.biYPelsPerMeter));
	output.write(reinterpret_cast<char*>(&dibheader.biClrUsed), sizeof(dibheader.biClrUsed));
	output.write(reinterpret_cast<char*>(&dibheader.biClrImportant), sizeof(dibheader.biClrImportant));

	// Pixel Array Write
	for(int y = 0; y <= image_y; y++) {
		for(int x = 0; x < image_x; x++) {
			if(DEBUG_LEVEL >= 2) { std::cout << "#Writing Pixel " << x << ' ' << y << "::" << std::endl; }
			output.write(reinterpret_cast<char*>(&pixelArray[(y * image_x) + x].rgbBlue), 1);
			output.write(reinterpret_cast<char*>(&pixelArray[(y * image_x) + x].rgbGreen), 1);
			output.write(reinterpret_cast<char*>(&pixelArray[(y * image_x) + x].rgbRed), 1);
		}
		for(int n = 0; n < (image_x % 4); n++) { // to ensure multipe of 4 requirement of bmp data
			int temp = 0;
			output.write(reinterpret_cast<char*>(&temp), 1);
		}
	}

	// Conclusion
	output.close();
	if(DEBUG_LEVEL >= 1) { std::cout << "#Closing File::" << std::endl; }
	return;
}

// Conclusion =====================================================================================

#endif

// END OF FILE ====================================================================================
