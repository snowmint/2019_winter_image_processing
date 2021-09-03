#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

void bigger_or_smaller(Mat img, Mat outimg);
int absi(int x);
int crgb(int x);
int linearInterpolate(int *mat, int n, double x1, double y1);

double lx = -1.0, ly = -1.0, lz = 1.0;
double mil = 130;
double env = 30;
#define NN 17
#define NF 8

void swapval(int *v1, int *v2)
{
	int dum;

	dum = *v1;
	*v1 = *v2;
	*v2 = dum;
}


double mat[16] =
{
	-1, 1,-1, 1,
	0, 0, 0, 1,
	1, 1, 1, 1,
	8, 4, 2, 1
};



double naiseki(double *x, double *y)
{
	int i;
	double res;

	res = 0.0;
	for (i = 0; i<4; i++) {
		res += (x[i] * y[i]);
	}
	return res;
}

double cubicFunc(double x)
{
	double ax;
	double res;

	if (x<0) ax = -x;
	else ax = x;

	if (0.0 <= ax && ax < 1.0) {
		res = 1.0 - 2.0*ax*ax + ax * ax*ax;
	}
	else if (1.0 <= ax && ax < 2.0) {
		res = 4.0 - 8.0*ax + 5.0*ax*ax - ax * ax*ax;
	}
	else {
		res = 0.0;
	}
	return res;
}

int biCubic(double x1, double y1, int *buf)
{
	int i, j;
	double fx[4], fy[4];
	double tmp[4], mat[4];
	double dx, dy;
	int px, py;
	int res;

	px = (int)x1;
	py = (int)y1;
	dx = x1 - (double)px;
	dy = y1 - (double)py;

	fx[0] = cubicFunc(1.0 + dx);
	fx[1] = cubicFunc(dx);
	fx[2] = cubicFunc(1.0 - dx);
	fx[3] = cubicFunc(2.0 - dx);

	fy[0] = cubicFunc(1.0 + dy);
	fy[1] = cubicFunc(dy);
	fy[2] = cubicFunc(1.0 - dy);
	fy[3] = cubicFunc(2.0 - dy);

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			mat[j] = (double)buf[j * 4 + i];
		}
		tmp[i] = naiseki(fy, mat);
	}
	res = (int)naiseki(tmp, fx);
	if (res<0) res = 0;
	else if (res>255) res = 255;
	return res;
}

/*©Ô®æ®Ô¤é*/
int interpolate(Mat img, double x, double y, Vec3b col)
{
	int px, py;
	px = (int)(x + 0.5);
	py = (int)(y + 0.5);
	if (py >= 0 && py < img.rows) {
		if (px >= 0 && px < img.cols) {
			return 1;
		}
	}
	return -1;
	//return getPixel(img, px, py, col);
}

int main() {

	Mat src, res1, res2, res3, res4, res5, res6;
	src = imread("cap3_07a.bmp"); //test image
	imshow("src", src);

	
	res1 = src.clone();
	
	int wx = src.cols;
    int hy = src.rows;

	cv::Mat bigger_img = cv::Mat::zeros(cv::Size(wx*1.2, hy*1.2), CV_8UC3);
	cv::Mat smaller_img = cv::Mat::zeros(cv::Size(wx * 0.8, hy * 0.8), CV_8UC3);
	
	bigger_or_smaller(src, bigger_img);
	imshow("Bigger", bigger_img);
	imwrite("bigger.bmp", bigger_img);

	bigger_or_smaller(src, smaller_img);
	imshow("Smaller", smaller_img);
	imwrite("smaller.bmp", smaller_img);

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
int getVal(int ed, int val, int con)
{
	int a, b;
	int v2;

	if (ed>255) ed = 255;
	v2 = (255 - 128)*val / 255 + 128;
	a = 255 - v2;
	b = -a * ed / 255 + 255;
	b = (b*con + val * (100 - con)) / 100;
	return b;
}

int linarPol(int c1, int c2, int c3, int c4, double xr, double yr)
{
	double d, e, f;
	int res;

	d = (double)c1*(1.0 - xr) + (double)c3*xr;
	e = (double)c2*(1.0 - xr) + (double)c4*xr;
	f = d * (1.0 - yr) + e * yr;
	res = (f + 0.5);
	if (res>255) res = 255;
	else if (res<0) res = 0;
	return res;
}

int getPatVal(int *mat, int n, int x, int y)
{
	if (x<0 || y<0 || x >= n || y >= n) return 0;

	return mat[x + y * n];
}

int linearInterpolate(int *mat, int n, double x1, double y1)
{
	int lx, ly;
	int adr;
	int mx, my, dep;
	int px, py;
	int nx, ny;
	double xrate, yrate;
	int col1, col2, col3, col4, col_n;

	nx = (int)(x1);
	ny = (int)(y1);
	px = (int)x1;
	py = (int)y1;
	xrate = x1 - (double)px;
	yrate = y1 - (double)py;
	col1 = getPatVal(mat, n, px, py);
	col2 = getPatVal(mat, n, px, py + 1);
	col3 = getPatVal(mat, n, px + 1, py);
	col4 = getPatVal(mat, n, px + 1, py + 1);
	return linarPol(col1, col2, col3, col4, xrate, yrate);
}

void bigger_or_smaller(Mat img, Mat outimg)
{
	int x, y;
	int x1, y1;
	int x2, y2;
	int rx1, ry1, rx2, ry2;
	Vec3b col, ncol;
	int h1, w1;
	int h2, w2;
	double fx, fy;
	int nfx, nfy;

	x1 = 0;
	y1 = 0;
	x2 = outimg.cols - 1;
	y2 = outimg.rows - 1;

	rx1 = 0;
	ry1 = 0;
	rx2 = img.cols - 1;
	ry2 = img.rows - 1;

	w1 = rx2 - rx1 + 1;
	h1 = ry2 - ry1 + 1;
	w2 = x2 - x1 + 1;
	h2 = y2 - y1 + 1;
	col.val[2] = col.val[1] = col.val[0] = 128;
	//col.r = col.g = col.b = 128;
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			fx = (double)(x - x1)*w1 / (double)w2 + rx1;
			fy = (double)(y - y1)*h1 / (double)h2 + ry1;
			if (interpolate(img, fx, fy, col) == 1) {//ncol.val[2] != -1 && ncol.val[1] != -1 && ncol.val[0] != -1
				int px = (int)(fx + 0.5);
				int py = (int)(fy + 0.5);
				if (py >= 0 && py < img.rows) {
					if (px >= 0 && px < img.cols) {
						ncol.val[2] = img.ptr<Vec3b>(py)[px].val[2];
						ncol.val[1] = img.ptr<Vec3b>(py)[px].val[1];
						ncol.val[0] = img.ptr<Vec3b>(py)[px].val[0];
					}
				}
				if (y >= 0 && y < outimg.rows) {
					if (x >= 0 && x < outimg.cols) {
						outimg.at<Vec3b>(y, x) = ncol;
					}
				}
			}
		}
	}
}