#include "iobmp.h"

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
