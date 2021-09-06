#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
#define PAI 3.1415926

using namespace std;
using namespace cv;


bool interpolate(Mat img, double x1, double y1, Vec3b *pix);
void swapval(int *v1, int *v2);
int resolve2Equ(double a, double b, double c, double *x1, double *x2);
int resolveEqu(double a, double b, double c, double d,
	double e, double f, double g, double h,
	double *xa, double *ya, double *xb, double *yb);
int linarPol(int c1, int c2, int c3, int c4, double xr, double yr);
int interpolate4_13(Mat img, double x, double y, Vec3b *col);
Mat effect_4_12(Mat img, int *px, int *py);
Mat effect_4_13(Mat img, int nx, int ny);


int main() {
	Mat src, src2, res1, res2, res3, res4, buf;
	Mat ans1, ans2;
	int px[4] = { 30,50,250,190 }, py[4] = { 30,200,250,20 };

	src = imread("Chrysanthemum.jpg");
	src2 = imread("cap4_06a.bmp");


	res1 = src.clone();
	res2 = src.clone();

	res3 = src2.clone();
	res4 = src2.clone();

	imshow("src", src);
	imshow("1-Free-form deformation", effect_4_12(res1, px, py));
	imwrite("1-Free-form_deformation.bmp", effect_4_12(res1, px, py));
									//nx, ny, 變形後圓柱的大小
	imshow("1-Cylindrical projection", effect_4_13(res2, 256, 256));
	imwrite("1-Cylindrical_projection.bmp", effect_4_13(res2, 256, 256));

	imshow("src2", src2);
	imshow("2-Free-form deformation", effect_4_12(res3, px, py));
	imwrite("2-Free-form_deformation.bmp", effect_4_12(res3, px, py));
									//nx, ny, 變形後圓柱的大小
	imshow("2-Cylindrical projection", effect_4_13(res4, 256, 256));
	imwrite("2-Cylindrical_projection.bmp", effect_4_13(res4, 256, 256));

	waitKey(0);
	return 0;
}

Mat effect_4_12(Mat img, int *px, int *py) 
{ //依照比例的自由變形
	Mat outimg;
	int x, y;
	int wt, ht;

	int i;
	Vec3b col, bcol;
	int h1, w1;
	int h2, w2;
	int rx1, ry1, rx2, ry2;
	double pa[4], pb[4];
	double aa, bb, cc, dd, ee, ff, gg, hh;
	int xa, ya, xb, yb;
	int xc, yc, xd, yd;

	rx1 = 0;
	ry1 = 0;
	rx2 = img.cols - 1;
	ry2 = img.rows - 1;

	wt = px[0];	ht = py[0];
	for (i = 1; i<4; i++) {
		x = px[i];
		y = py[i];
		if (x>wt) wt = x;
		if (y>ht) ht = y;
	}
	wt++;
	ht++;
	outimg = cv::Mat::zeros(cv::Size(wt, ht), CV_8UC3);

	xa = (double)px[0];
	ya = (double)py[0];
	xb = (double)px[1];
	yb = (double)py[1];
	xc = (double)px[2];
	yc = (double)py[2];
	xd = (double)px[3];
	yd = (double)py[3];

	aa = (double)(xc - xd - xb + xa);
	bb = (double)(xb - xa);
	cc = (double)(xd - xa);
	ff = (double)(yc - yd - yb + ya);
	gg = (double)(yb - ya);
	hh = (double)(yd - ya);


	bcol.val[2] = bcol.val[1] = bcol.val[0] = 128;
	for (y = 0; y<ht; y++) {
		for (x = 0; x<wt; x++) {
			double ss1, tt1, ss2, tt2;
			double xx, yy;
			double ttx, tty;

			dd = (double)(x - xa);
			ee = (double)(y - ya);

			if (!resolveEqu(aa, bb, cc, dd, ee, ff, gg, hh, &ss1, &tt1, &ss2, &tt2)) {
				printf("NO!\n");
				return img;
			}
			if (ss1 >= 0 && tt1 >= 0 && ss1 <= 1.0 && tt1 <= 1.0) {
				ttx = ss1 * (rx2 - rx1) + rx1;
				tty = tt1 * (ry2 - ry1) + ry1;
			}
			else if (ss2 >= 0 && tt2 >= 0 && ss2 <= 1.0 && tt2 <= 1.0) {
				ttx = ss2 * (rx2 - rx1) + rx1;
				tty = tt2 * (ry2 - ry1) + ry1;
			}
			else {
				//setPixel(outimg, x, y, &bcol);
				outimg.at<Vec3b>(y, x) = bcol;
				continue;
			}
			if (interpolate(img, ttx, tty, &col)) {
				//setPixel(outimg, x, y, &col);
				outimg.at<Vec3b>(y, x) = col;
			}
			else {
				//setPixel(outimg, x, y, &bcol);
				outimg.at<Vec3b>(y, x) = bcol;
			}
		}
	}
	return outimg;
}

Mat effect_4_13(Mat img, int nx, int ny)
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
	double theta;
	double ro;
	double wo;
	double ww;
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
	ro = (double)w2 / 2.0;
	col.val[2] = col.val[1] = col.val[0] = 128;
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			ww = (double)(x - x1);
			if (ro - ww != 0.0)
				theta = atan(sqrt(2.0*ro*ww - ww * ww) / (ro - ww));
			else
				theta = PAI / 2.0;
			if (theta<0.0) theta += PAI;
			fx = (theta*(double)w1) / PAI + rx1;
			fy = (y - y1)*h1 / (double)h2 + ry1;
			if (interpolate4_13(img, fx, fy, &col)) {
				//setPixel(outimg, x, y, &col);
				if (y >= 0 && y < outimg.rows) {
					if (x >= 0 && x < outimg.cols) {
						outimg.at<Vec3b>(y, x) = col;
					}
				}
			}
		}
	}
	return outimg;
}

bool interpolate(Mat img, double x1, double y1, Vec3b *pix)
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
void swapval(int *v1, int *v2)
{
	int dum;

	dum = *v1;
	*v1 = *v2;
	*v2 = dum;
}

int resolve2Equ(double a, double b, double c, double *x1, double *x2)
{
	double dd;

	if (a == 0.0) {
		if (c == 0.0) return 0;//F
		*x1 = -b / c;
		*x2 = -b / c;
		return 1;//T
	}
	dd = b * b - 4.0*a*c;
	*x1 = 0.0;
	*x2 = 0.0;
	if (dd<0.0) {
		return 0;//F
	}
	*x1 = (-b + sqrt(dd)) / (2.0*a);
	*x2 = (-b - sqrt(dd)) / (2.0*a);
	return 1;//T
}

int resolveEqu(double a, double b, double c, double d,
	double e, double f, double g, double h,
	double *xa, double *ya, double *xb, double *yb)
{
	double p1, p2, p3;
	double x1, x2;
	double y1, y2;
	double tm;
	char cbuf[256];

	p1 = (double)(a*h - f * c);
	p2 = (double)(d*f - c * g + b * h - a * e);
	p3 = (double)(d*g - b * e);
	if (p1 == 0.0) {
		*xa = d / b;
		*ya = (b*e - d * g) / (b*h + d * f);
		*xb = -1000;
		*yb = -1000;
		return 1;//T
	}
	if (!resolve2Equ(p1, p2, p3, ya, yb)) {
		*xa = -1000;
		*xb = -1000;
		return 1;//T
	}
	tm = (a*(*ya) + b);
	if (tm)
		*xa = (d - c * (*ya)) / tm;
	else {
		*xa = -1000;
	}
	tm = (a*(*yb) + b);
	if (tm)
		*xb = (d - c * (*yb)) / tm;
	else
		*xb = -1000.0;
	return 1;//T
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

int interpolate4_13(Mat img, double x, double y, Vec3b *col)
{
	int lx, ly;
	int val;
	int adr;
	int mx, my, dep;
	int px, py;
	double xrate, yrate;
	Vec3b col1, col2, col3, col4;

	px = (int)x;
	py = (int)y;
	xrate = x - (double)px;
	yrate = y - (double)py;

	if (py >= 0 && py < img.rows) {
		if (px >= 0 && px < img.cols) {
			col1.val[2] = img.ptr<Vec3b>(py)[px].val[2];
			col1.val[1] = img.ptr<Vec3b>(py)[px].val[1];
			col1.val[0] = img.ptr<Vec3b>(py)[px].val[0];
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}

	if (py + 1 >= 0 && py + 1 < img.rows) {
		if (px >= 0 && px < img.cols) {
			col2.val[2] = img.ptr<Vec3b>(py + 1)[px].val[2];
			col2.val[1] = img.ptr<Vec3b>(py + 1)[px].val[1];
			col2.val[0] = img.ptr<Vec3b>(py + 1)[px].val[0];
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
	if (py >= 0 && py < img.rows) {
		if (px + 1 >= 0 && px + 1 < img.cols) {
			col3.val[2] = img.ptr<Vec3b>(py)[px + 1].val[2];
			col3.val[1] = img.ptr<Vec3b>(py)[px + 1].val[1];
			col3.val[0] = img.ptr<Vec3b>(py)[px + 1].val[0];
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}

	if (py + 1 >= 0 && py + 1 < img.rows) {
		if (px + 1 >= 0 && px + 1 < img.cols) {
			col4.val[2] = img.ptr<Vec3b>(py + 1)[px + 1].val[2];
			col4.val[1] = img.ptr<Vec3b>(py + 1)[px + 1].val[1];
			col4.val[0] = img.ptr<Vec3b>(py + 1)[px + 1].val[0];
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}

	col->val[2] = linarPol(col1.val[2], col2.val[2], col3.val[2], col4.val[2], xrate, yrate);
	col->val[1] = linarPol(col1.val[1], col2.val[1], col3.val[1], col4.val[1], xrate, yrate);
	col->val[0] = linarPol(col1.val[0], col2.val[0], col3.val[0], col4.val[0], xrate, yrate);
	return 1;
}
