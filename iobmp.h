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

// Conclusion =====================================================================================

#endif

// END OF FILE ====================================================================================
