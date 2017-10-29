#include "iobmp.h"

// Finalize and output BMP object =================================================================

void bmpFile::BMPfinalize(std::string file) {
	// this function should be called to finalize pixel data and write bmpfile

	dibheader.biSizeImage = (image_x * 3 + image_x % 4) * image_y;
	header.bfSize = dibheader.biSizeImage + 54;

	fileName = file;

	writeFile();
	return;
}

// Private Member Write ===========================================================================

void bmpFile::writeFile() {
	// this function begins the writeout process of the class data

	std::ofstream output;
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
	return;
}
