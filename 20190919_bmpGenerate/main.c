#include "image.h"

int main(int ac, char *av[]) {
	ImageData *img;
	int res;
	int x, y, mx, my;
	Pixel pix1, pix2;

	pix1.r = 255;
	pix1.g = 255;
	pix1.b = 255;
	pix2.r = 0;
	pix2.g = 0;
	pix2.b = 0;
	img = createImage(100, 100, 24);
	//for (y = 0; y<100; y++) {
	//	for (x = 0; x<100; x++) {
	//		if (x % 2 == y % 2) {
	//			setPixel(img, x, y, &pix1);
	//		}
	//		else {
	//			setPixel(img, x, y, &pix2);
	//		}
	//	}
	//}
	for (y = 0; y<100; y++) {
		for (x = 0; x<100; x++) {
			if (x >= 40 && x <= 60 && y == 20) {
				setPixel(img, x, y, &pix2);
			}
			else if (x >= 40 && x <= 60 && y == 80) {
				setPixel(img, x, y, &pix2);
			}
			if (y >= 20 && y <= 80 && x == 40) {
				setPixel(img, x, y, &pix2);
			}
			else if (y >= 20 && y <= 80 && x == 60) {
				setPixel(img, x, y, &pix2);
			}
		}
	}
	writeBMPfile("out2.bmp", img);
	disposeImage(img);

	return 0;
}
