#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
#include "image.h"

using namespace std;
using namespace cv;

using namespace std;
using namespace cv;
Mat effect_4_5(Mat input, int nx, int ny);
Mat effect_4_7(Mat input, int wt, int ht, double a, double b, double c, double d, double e, double f);
Mat effect_4_9(Mat img, Mat outimg_p, double xgap, double ygap);

int biCubic(double x1, double y1, int *buf);
double cubicFunc(double x);
double naiseki(double *x, double *y);
int interpolateFor4_5(Mat img, double x1, double y1, Vec3b *pix);
int interpolateFor4_7(Mat img, double x1, double y1, Vec3b *pix);
bool interpolate4_9(Mat img, double x1, double y1, Vec3b* pix);

bool interpolate4_9(Mat img, double x1, double y1, Vec3b *pix)
{
	int x, y;

	x = (int)(x1 + 0.5);
	y = (int)(y1 + 0.5);
	if (y >= 0 && y < img.rows) {
		if (x >= 0 && x < img.cols) {
			pix->val[2] = img.ptr<Vec3b>(y)[x].val[2];
			pix->val[1] = img.ptr<Vec3b>(y)[x].val[1];
			pix->val[0] = img.ptr<Vec3b>(y)[x].val[0];
			return true;
		}
	}
	return false;
}

int main() {
	Mat src, res1, res2, buf;
	Mat ans1, ans2;
	
	src = imread("Lenna.bmp"); //test image2

	imshow("src", src);
	imshow("resize", effect_4_5(src, 200, 200));
	imshow("affine deformation", effect_4_7(src, 700, 700, 1, -0.5, 0, 0, 1, 0));
	imwrite("resize_with_specify_width_height.bmp", effect_4_5(src, 200, 200));
	imwrite("affine_deformation.bmp", effect_4_7(src, 700, 700, 1, -0.5, 0, 0, 1, 0));
	
	Mat outimg;
	imshow("4-9", effect_4_9(src, outimg, 200, 200));
	imwrite("shear_transformation.bmp", effect_4_9(src, outimg, 200, 200));
	waitKey(0);
	return 0;
}
Mat effect_4_5(Mat img, int nx, int ny)
{
	Mat outimg;
	outimg.create(ny, nx, CV_8UC3);
	int x, y;
	int x1, y1;
	int x2, y2;
	int rx1, ry1, rx2, ry2;
	Vec3b col;
	int h1, w1;
	int h2, w2;
	double fx, fy;

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
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			fx = (x - x1)*w1 / (double)w2 + rx1;
			fy = (y - y1)*h1 / (double)h2 + ry1;
			if (interpolateFor4_5(img, fx, fy, &col)) {
				outimg.at<Vec3b>(y, x) = col;
			}
		}
	}

	return outimg;
}

Mat effect_4_7(Mat img, int wt, int ht, double a, double b, double c, double d, double e, double f) 
{
	Mat outimg;
	outimg.create(ht, wt, CV_8UC3);

	int x, y;
	double x1, y1;
	double x2, y2;
	Vec3b col, bcol;
	int h1, w1;
	int h2, w2;
	int tx, ty;
	double xx, yy;
	double px[4], py[4];
	int i;

	int dx, dy;
	double n, m;
	double det;

	bcol.val[2] = bcol.val[1] = bcol.val[0] = 128;
	for (y = 0; y<ht; y++) {
		for (x = 0; x<wt; x++) {
			tx = (double)(x);
			ty = (double)(y);
			xx = (a*tx + b * ty + c);
			yy = (d*tx + e * ty + f);
			if (interpolateFor4_7(img, xx, yy, &col)) {
				outimg.at<Vec3b>(y, x) = col;
			}
			else {
				outimg.at<Vec3b>(y, x) = bcol;
			}
		}
	}
	return outimg;
}

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

int interpolateFor4_5(Mat img, double x1, double y1, Vec3b *pix)
{
	int px, py;
	int val;
	int adr2;
	int r1, g1, b1, o1;
	int lx2, ly2;
	Vec3b col2;
	int bufR[16] = { 0 }, bufG[16] = { 0 }, bufB[16] = { 0 };
	px = (int)x1;
	py = (int)y1;

	for (ly2 = 0; ly2<4; ly2++) {
		adr2 = ly2 * 4;
		for (lx2 = 0; lx2<4; lx2++) {
			if (py + ly2 - 1 >= 0 && py + ly2 - 1 < img.rows) {
				if (px + lx2 - 1 >= 0 && px + lx2 - 1 < img.cols) {
					col2.val[2] = img.ptr<Vec3b>(py + ly2 - 1)[px + lx2 - 1].val[2];
					col2.val[1] = img.ptr<Vec3b>(py + ly2 - 1)[px + lx2 - 1].val[1];
					col2.val[0] = img.ptr<Vec3b>(py + ly2 - 1)[px + lx2 - 1].val[0];
					bufR[adr2] = col2.val[2];
					bufG[adr2] = col2.val[1];
					bufB[adr2] = col2.val[0];
				}
			}
			adr2++;
		}
	}
	r1 = biCubic(x1, y1, bufR);
	g1 = biCubic(x1, y1, bufG);
	b1 = biCubic(x1, y1, bufB);

	pix->val[2] = r1;
	pix->val[1] = g1;
	pix->val[0] = b1;
	return true;
}

int interpolateFor4_7(Mat img, double x1, double y1, Vec3b *pix)
{
	int x, y;

	x = (int)(x1 + 0.5);
	y = (int)(y1 + 0.5);
	if (y >= 0 && y < img.rows) {
		if (x >= 0 && x < img.cols) {
			pix->val[2] = img.ptr<Vec3b>(y)[x].val[2];
			pix->val[1] = img.ptr<Vec3b>(y)[x].val[1];
			pix->val[0] = img.ptr<Vec3b>(y)[x].val[0];
			return true;
		}
	}
	return false;
}


Mat effect_4_9(Mat img, Mat outimg, double xgap, double ygap)
{
	int x, y;
	double x1, y1;
	double x2, y2;
	int wt, ht;
	Vec3b col, bcol;
	int h1, w1;
	int h2, w2;
	int tx, ty;
	double xx, yy;
	double px[4], py[4];
	int i;

	int dx, dy;
	double n, m;
	double aa, bb, cc, dd;
	double det;

	px[0] = 0;
	py[0] = 0;

	px[1] = (double)((double)img.cols - (double)1);
	py[1] = 0;

	px[2] = (double)((double)img.cols - (double)1);
	py[2] = (double)((double)img.rows - (double)1);

	px[3] = 0;
	py[3] = (double)((double)img.rows - (double)1);

	x1 = px[0];	y1 = py[0];
	x2 = px[2] + xgap;	y2 = py[2] + ygap;

	wt = (int)(x2 - x1 + 0.5);
	ht = (int)(y2 - y1 + 2.5);

	outimg.create(ht, wt, CV_8UC3);

	n = ygap / px[2];
	m = xgap / py[2];

	aa = 1.0;
	bb = -m;
	cc = -n;
	dd = 1.0;
	det = aa * dd - bb * cc;

	bcol[2] = bcol[1] = bcol[0] = 128;

	bcol.val[2] = bcol.val[1] = bcol.val[0] = 128;
	for (y = 0; y < ht; y++) {
		for (x = 0; x < wt; x++) {
			tx = (double)(x);
			ty = (double)(y);
			xx = (aa * tx + bb * ty) / det;
			yy = (cc * tx + dd * ty) / det;
			if (interpolateFor4_7(img, xx, yy, &col)) {
				outimg.at<Vec3b>(y, x) = col;
				printf("1");
			}
			else {
				outimg.at<Vec3b>(y, x) = bcol;
				printf("2");
			}
		}
	}
	return outimg;
}

