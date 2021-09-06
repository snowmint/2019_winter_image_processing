#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
#define PAI 3.1415926
#define SCALE 255.0
#define hSCALE 256.0
#define GETA 2.0
#define hGETA 2.0

#define sgn(x) ((x)<0 ? (-1.0):(1.0))
#define CINT(x)  ((int)((x)+sgn(x)*0.5))
#define FL float

#define CMAX 255
#define REGIONSIZE 10

using namespace std;
using namespace cv;

void effect_6_4(Mat img1, Mat img2, Mat outimg, int rr, int gg, int bb, int r1, int r2);
void effect_6_5(Mat img1, Mat img2, Mat outimg);

int RList[256] = { 0 };
int GList[256] = { 0 };
int BList[256] = { 0 };

int main() {
	Mat src, src1;
	Mat res, res1;
	int i = 0;
	string array[14] = { "output0-0.jpg", "output0-1.jpg", "output0-2.jpg", "output0-3.jpg", "output0-4.jpg", "output0-5.jpg", "output0-6.jpg", "output1.jpg", "output2.jpg", "output3.jpg", "output4.jpg", "output5.jpg", "output6.jpg", "output7.jpg" };

	while(1) {
		src = imread(array[i]);
		imshow("src", src);
		if (i == 13) i = 0;
		else i++;
		waitKey(60);
	}

	waitKey(0);
	return 0;
}

int blend(int c1, int c2, int a)
{
	int v;

	v = (c1*a + (CMAX - a)*c2) / CMAX;
	if (v<0) v = 0;
	if (v>CMAX) v = CMAX;
	return v;
}

int getAlpha(Vec3b *rgb, int rr, int gg, int bb, int r1, int r2)
{
	int dr, dg, db;
	int v1, v2;
	double vv;

	dr = (rgb->val[2] - rr);
	dg = (rgb->val[1] - gg);
	db = (rgb->val[0] - bb);
	v1 = dr * dr + dg * dg + db * db;
	vv = sqrt((double)v1);
	if (vv <= (double)r1) return CMAX;
	if (vv >= (double)r2) return 0;
	return (int)((double)CMAX*(vv - (double)r2) / (double)(r1 - r2));
}

void effect_6_4(Mat img1, Mat img2, Mat outimg, int rr, int gg, int bb, int r1, int r2)
{
	Vec3b rgb1, rgb2, rgb3;
	int alpha;
	int size;
	int xx, yy, mx, my;
	int i, res;

	rr = 70;
	gg = 70;
	bb = 70;
	r1 = 60;
	r2 = 170;
	printf("READ!\n");
	mx = img2.cols;//width
	my = img2.rows;//height
	if (mx>img1.cols) mx = img1.cols;
	if (my>img1.rows) my = img1.rows;
	for (yy = 0; yy < my; yy++) {
		for (xx = 0; xx < mx; xx++) {
			//getPixel(img1, xx, yy, &rgb1);
			//getPixel(img2, xx, yy, &rgb2);
			rgb1.val[2] = img1.ptr<Vec3b>(yy)[xx].val[2];
			rgb1.val[1] = img1.ptr<Vec3b>(yy)[xx].val[1];
			rgb1.val[0] = img1.ptr<Vec3b>(yy)[xx].val[0];

			rgb2.val[2] = img2.ptr<Vec3b>(yy)[xx].val[2];
			rgb2.val[1] = img2.ptr<Vec3b>(yy)[xx].val[1];
			rgb2.val[0] = img2.ptr<Vec3b>(yy)[xx].val[0];

			alpha = getAlpha(&rgb2, rr, gg, bb, r1, r2);
			rgb3.val[2] = blend(rgb1.val[2], rgb2.val[2], alpha);
			rgb3.val[1] = blend(rgb1.val[1], rgb2.val[1], alpha);
			rgb3.val[0] = blend(rgb1.val[0], rgb2.val[0], alpha);


			outimg.ptr<Vec3b>(yy)[xx].val[2] = rgb3.val[2];
			outimg.ptr<Vec3b>(yy)[xx].val[1] = rgb3.val[1];
			outimg.ptr<Vec3b>(yy)[xx].val[0] = rgb3.val[0];
		}
	}
	//writeBMPfile(av[8], img1);
	//disposeImage(img1);
	//disposeImage(img2);
}


int inRegion(int x, int y, int bios)
{
	int wx;
	int range = REGIONSIZE;
	wx = x / (range+bios);
	//printf("bios = %d\n", bios);
	return wx % 2;
}
void effect_6_5(Mat img1, Mat img2, Mat outimg)
{
	Vec3b rgb1, rgb2, rgb3;
	int alpha;
	int rr, gg, bb, r1;
	int size;
	int xx, yy, mx, my;
	int i;
	int res;
	int bios = 1000;//200 150 100 50 30 10

	mx = img2.cols;
	my = img2.rows;
	if (mx>img1.cols) mx = img1.cols;
	if (my>img1.rows) my = img1.rows;
	for (yy = 0; yy < my; yy++) {
		//bios += (int)(yy/1500);//1500 1000 600 500 300 100 50

		for (xx = 0; xx < mx; xx++) {
			alpha = (xx * 255 + mx / 2) / mx;
			//getPixel(img1, xx, yy, &rgb1);
			//getPixel(img2, xx, yy, &rgb2);
			rgb1.val[2] = img1.ptr<Vec3b>(yy)[xx].val[2];
			rgb1.val[1] = img1.ptr<Vec3b>(yy)[xx].val[1];
			rgb1.val[0] = img1.ptr<Vec3b>(yy)[xx].val[0];

			rgb2.val[2] = img2.ptr<Vec3b>(yy)[xx].val[2];
			rgb2.val[1] = img2.ptr<Vec3b>(yy)[xx].val[1];
			rgb2.val[0] = img2.ptr<Vec3b>(yy)[xx].val[0];
			if (inRegion(xx, yy, bios) == 0) {
				rgb3 = rgb1;//rgb1;
			}
			else {
				rgb3 = rgb2;//rgb2;
			}

			outimg.ptr<Vec3b>(yy)[xx].val[2] = rgb3.val[2];
			outimg.ptr<Vec3b>(yy)[xx].val[1] = rgb3.val[1];
			outimg.ptr<Vec3b>(yy)[xx].val[0] = rgb3.val[0];
			
		}
	}
}