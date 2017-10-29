
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
