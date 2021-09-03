#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

void effect_3_14(Mat img, Mat outimg, double ef);
void effect_3_19(Mat img, Mat outimg);
int absi(int x);
int crgb(int x);
void adjustLightSourse();
int lighting(int g, double dx, double dy, double dz, double x, double y, double z);

double lx = -1.0, ly = -1.0, lz = 1.0;
double mil = 130;
double env = 30;
#define NN 17
#define NF 8

int main() {

	Mat src, res1, res2;
	src = imread("cap3_07a.bmp"); //test image
	imshow("src", src);


	res1 = src.clone();
	res2 = src.clone();


	effect_3_14(src, res1, 50);
	effect_3_19(src, res2);

	imshow("radial blur ", res1);
	imshow("relief transformation", res2);

	imwrite("radial_blur.bmp", res1);
	imwrite("relief_transformation.bmp", res2);

	waitKey(0);
	return 0;
}
int absi(int x)
{
	if (x < 0) return -x;
	return x;
}

int crgb(int x)
{
	if (x < 0) return 0;
	if (x > 255) return 255;
	return x;
}

void effect_3_14(Mat img, Mat outimg, double ef)// 放射狀模糊化
{
	int val;
	double rate;
	int i;
	int x, y;
	int xx, yy;
	int rr, gg, bb, oo;
	int endn;
	Vec3b col, ncol;
	int mx;
	int pat_sum, pat;
	double rad, dis, disI;
	double ox, oy;
	double disMAX;
	double dx, dy;
	int x1, y1, x2, y2;

	x1 = 0;
	y1 = 0;
	x2 = img.cols - 1;
	y2 = img.rows - 1;

	ox = (double)(x2 - x1) / 2.0;
	oy = (double)(y2 - y1) / 2.0;
	dx = (double)x2 - ox;
	dy = (double)y2 - oy;
	disMAX = sqrt(dx*dx + dy * dy);

	for (y = 0; y <= y2; y++) {
		for (x = 0; x <= x2; x++) {
			rr = gg = bb = oo = 0;
			dx = (double)x - ox;
			dy = (double)y - oy;
			if (dx != 0.0) rad = atan(dy / dx);
			else rad = 3.14159265 / 2.0;
			dis = sqrt(dx*dx + dy * dy);
			rate = ef * dis / disMAX;
			rate /= ((double)NF);
			pat_sum = 0;

			for (i = 0; i<NN; i++) {
				if (i == NF) pat = 3;
				else pat = 1;
				disI = (double)(i - NF)*rate;
				xx = (int)(disI*cos(rad)) + x;
				yy = (int)(disI*sin(rad)) + y;
				if (yy >= 0 && yy < img.rows) {
					if (xx >= 0 && xx < img.cols) {
						ncol.val[2] = img.ptr<Vec3b>(yy)[xx].val[2];
						ncol.val[1] = img.ptr<Vec3b>(yy)[xx].val[1];
						ncol.val[0] = img.ptr<Vec3b>(yy)[xx].val[0];
						rr += ncol.val[2] * pat;
						gg += ncol.val[1] * pat;
						bb += ncol.val[0] * pat;
						oo += pat;
						pat_sum += pat;
					}
				}
			}
			if (y >= 0 && y < img.rows) {
				if (x  >= 0 && x< img.cols) {
					col.val[2] = rr / (oo);
					col.val[1] = gg / (oo);
					col.val[0] = bb / (oo);
					outimg.at<Vec3b>(y, x) = col;
				}
			}
		}
	}
}
void effect_3_19(Mat img, Mat outimg) //浮雕處理
{
	int val;
	int sobel1[9] = {
		1, 0,-1,
		2, 0,-2,
		1, 0,-1 };
	int sobel2[9] = {
		1, 2, 1,
		0, 0, 0,
		-1,-2,-1 };
	int x, y;
	int xx, yy;
	double dx, dy, dz;
	int hh;
	int du, dd, dl, dr;
	int endn;
	int c1, c2;
	int rrx, ggx, bbx;
	int rry, ggy, bby;
	int rr, gg, bb;
	Vec3b col, ncol, ocol;
	int *sobel;
	int sadr;
	int rate, vh;
	int gray;
	int x1, y1, x2, y2;

	x1 = 0;
	y1 = 0;
	x2 = img.cols - 1;
	y2 = img.rows - 1;

	adjustLightSourse();
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			rrx = ggx = bbx = 0;
			rry = ggy = bby = 0;
			sadr = 0;
			for (yy = 0; yy<3; yy++) {
				for (xx = 0; xx<3; xx++) {
					if (y + yy-1 >= 0 && y + yy -1 < img.rows) {
						if (x + xx - 1 >= 0 && x + xx -1 < img.cols) {
							col.val[2] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[2];
							col.val[1] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[1];
							col.val[0] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[0];
							rr = col.val[2];
							gg = col.val[1];
							bb = col.val[0];
							gray = (bb * 28 + 77 * rr + gg * 151) / 256;
							rrx += gray * sobel1[sadr];
							rry += gray * sobel2[sadr];
						}
					}
					sadr++;
				}
			}
			dx = (double)rrx;
			dy = (double)rry;
			dz = 16.0;
			if (y + yy - 1 >= 0 && y + yy - 1 < img.rows) {
				if (x + xx - 1 >= 0 && x + xx - 1 < img.cols) {
					ncol.val[2] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[2];
					ncol.val[1] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[1];
					ncol.val[0] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[0];
					rr = lighting(ncol.val[2], dx, dy, dz, (double)(x - x1), (double)(y - y1), 0.0);
					gg = lighting(ncol.val[1], dx, dy, dz, (double)(x - x1), (double)(y - y1), 0.0);
					bb = lighting(ncol.val[0], dx, dy, dz, (double)(x - x1), (double)(y - y1), 0.0);
					ocol.val[2] = rr;
					ocol.val[1] = gg;
					ocol.val[0] = bb;
					if (y >= 0 && y < img.rows) {
						if (x >= 0 && x< img.cols) {
							outimg.at<Vec3b>(y, x) = ocol;
						}
					}
				}
			}
		}
	}
}
void adjustLightSourse() {
	double abl;

	abl = sqrt(lx*lx + ly * ly + lz * lz);
	lx = lx / abl;
	ly = ly / abl;
	lz = lz / abl;
}
int lighting(int g, double dx, double dy, double dz, double x, double y, double z)
{
	double cosT, cosT2;
	double rx, ry, rz;
	double abl;
	double env2;
	int res;

	abl = sqrt(dx*dx + dy * dy + dz * dz);
	dx = dx / abl;
	dy = dy / abl;
	dz = dz / abl;
	cosT = dx * lx + dy * ly + dz * lz;
	rz = 2.0*cosT*dz - lz;
	if (rz>0.0)
		cosT2 = rz * rz*rz*rz*rz*rz*rz*rz*rz*rz*rz*rz;// n=12;
	else
		cosT2 = 0;
	if (cosT<0.0) cosT = 0.0;
	env2 = env * 255.0 / 100.0;
	res = (int)((cosT*255.0 + env2)*(double)g / 255.0);
	if (res>g) res = g;
	res += (mil*cosT2);
	if (res>255) res = 255;
	if (res<0) res = 0;
	return res;
}
