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



using namespace std;
using namespace cv;

void HSVtoRGB(int h, int s, int v, int *r, int *g, int *b);
void RGBtoHSV(int r, int g, int b, int *h, int *s, int *v);
void effect_5_3(Mat img, Mat outimg, int h_flag, int s_flag, int v_flag); // 從 HSV 中抽出任意成分 : 此處針對膚色區域進行提取
void effect_5_8(Mat img, Mat outimg); // 針對 V 成分的效果處理 : 透過明暗(V)擷取輪廓

int main() {
	Mat src;
	Mat res, res1;

	//src = imread("Chrysanthemum.jpg");
	src = imread("Diversity.jpg");
	res = src.clone();
	res1 = src.clone();

	imshow("src", src);

	effect_5_3(src, res, 0, 200, 200);
	effect_5_8(src, res1);

	imshow("Skin capture", res);
	imwrite("Diversity_Skin_capture.jpg", res);
	imshow("Use V(Value) get contour", res1);
	imwrite("Diversity_Use_V(Value)_get_contour.jpg", res1);

	waitKey(0);
	return 0;
}

void effect_5_3(Mat img, Mat outimg, int h_flag, int s_flag, int v_flag)
{
	int x, y;
	int i;
	int val;
	int rr, gg, bb;
	int hh, ss, vv;
	Vec3b pix;

	for (y = 0; y<img.cols; y++) {
		for (x = 0; x<img.rows; x++) {
			if (y >= 0 && y < img.rows) {
				if (x >= 0 && x < img.cols) {
					pix.val[2] = img.ptr<Vec3b>(y)[x].val[2];
					pix.val[1] = img.ptr<Vec3b>(y)[x].val[1];
					pix.val[0] = img.ptr<Vec3b>(y)[x].val[0];

					rr = pix.val[2];
					gg = pix.val[1];
					bb = pix.val[0];

					RGBtoHSV(rr, gg, bb, &hh, &ss, &vv);
					if (((hh >= 50) && (hh <= 250)) && (vv >= 0.8 * 255)) {
						HSVtoRGB(hh, ss, vv, &rr, &gg, &bb);
						pix.val[2] = 255;
						pix.val[1] = 255;
						pix.val[0] = 255;
					}
					else {
						HSVtoRGB(hh, ss, vv, &rr, &gg, &bb);
						pix.val[2] = 0;
						pix.val[1] = 0;
						pix.val[0] = 0;
					}

					outimg.ptr<Vec3b>(y)[x].val[2] = pix.val[2];
					outimg.ptr<Vec3b>(y)[x].val[1] = pix.val[1];
					outimg.ptr<Vec3b>(y)[x].val[0] = pix.val[0];
				}
			}
		}
	}

}
void effect_5_8(Mat img, Mat outimg)
{
	int mask[9] =
	{
		-1,-2,-1,
		0, 0, 0,
		1, 2, 1 };
	int val;
	int x, y;
	int xx, yy;
	int rr, gg, bb;
	int hh, ss, vv;
	int sum;
	Vec3b col;

	for (y = 0; y<img.cols; y++) {
		for (x = 0; x<img.rows; x++) {
			sum = 0;
			for (yy = 0; yy<3; yy++) {
				for (xx = 0; xx<3; xx++) {
					if (y + yy - 1 >= 0 && y + yy - 1 < img.rows) {
						if (x + xx - 1 >= 0 && x + xx - 1 < img.cols) {
							col.val[2] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[2];
							col.val[1] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[1];
							col.val[0] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[0];

							rr = col.val[2];
							gg = col.val[1];
							bb = col.val[0];

							RGBtoHSV(rr, gg, bb, &hh, &ss, &vv);
							sum += vv * mask[xx + yy * 3];
						}
					}
				}
			}
			if (sum < 0) sum = -sum;
			if (sum > 511) sum = 511;
			if (y >= 0 && y < img.rows) {
				if (x >= 0 && x < img.cols) {
					col.val[2] = img.ptr<Vec3b>(y)[x].val[2];
					col.val[1] = img.ptr<Vec3b>(y)[x].val[1];
					col.val[0] = img.ptr<Vec3b>(y)[x].val[0];

					RGBtoHSV(col.val[2], col.val[1], col.val[0], &hh, &ss, &vv);
					HSVtoRGB(hh, ss, sum, &rr, &gg, &bb);
					col.val[2] = rr;
					col.val[1] = gg;
					col.val[0] = bb;

					outimg.at<Vec3b>(y, x) = col;
				}
			}
		}
	}
}


void RGBtoHSV(int r, int g, int b, int *h, int *s, int *v)
{
	FL rr, gg, bb;
	FL hh, ss, vv;
	FL cmax, cmin, cdes;

	rr = (FL)r;
	gg = (FL)g;
	bb = (FL)b;

	if (rr>gg) cmax = rr;
	else cmax = gg;
	if (bb>cmax) cmax = bb;

	if (rr<gg) cmin = rr;
	else cmin = gg;
	if (bb<cmin) cmin = bb;

	cdes = cmax - cmin;
	vv = cmax;
	if (cdes != 0.0) {
		ss = cdes * SCALE / cmax;
		if (cmax == rr) {
			hh = (gg - bb)*SCALE / cdes;
		}
		else if (cmax == gg) {
			hh = (bb - rr)*SCALE / cdes + 2.0*hSCALE;
		}
		else {
			hh = (rr - gg)*SCALE / cdes + 4.0*hSCALE;
		}
	}
	else if (cmax != 0.0) {
		ss = cdes * SCALE / cmax;
		hh = 0.0;
	}
	else {
		ss = 0.0;
		hh = 0.0;
	}
	if (hh<0.0) hh += 6.0*hSCALE;

	*h = CINT(hh*hGETA);
	*s = CINT(ss*GETA);
	*v = CINT(vv*GETA);
}

void HSVtoRGB(int h, int s, int v, int *r, int *g, int *b)
{
	FL hh, ss, vv;
	FL rr, gg, bb;

	if (h == 6 * hGETA*(hSCALE)) h = 0;
	hh = (FL)h / hGETA;
	ss = (FL)s / GETA;
	vv = (FL)v / GETA;

	switch ((int)(hh / hSCALE)) {
	case 0:
		rr = 1.0*SCALE;
		gg = hh;
		bb = 0.0;
		break;
	case 1:
		rr = 2.0*hSCALE - hh;
		gg = 1.0*SCALE;
		bb = 0.0;
		break;
	case 2:
		rr = 0.0;
		gg = 1.0*SCALE;
		bb = hh - 2.0*hSCALE;
		break;
	case 3:
		rr = 0;
		gg = 4.0*hSCALE - hh;
		bb = 1.0*SCALE;
		break;
	case 4:
		rr = hh - 4.0*hSCALE;
		gg = 0;
		bb = 1.0*SCALE;
		break;
	case 5:
		rr = 1.0*SCALE;
		gg = 0;
		bb = 6.0*hSCALE - hh;
		break;
	}

	rr = (rr + (1.0*SCALE - rr)*(1.0*SCALE - ss) / (SCALE))*vv / SCALE;
	gg = (gg + (1.0*SCALE - gg)*(1.0*SCALE - ss) / (SCALE))*vv / SCALE;
	bb = (bb + (1.0*SCALE - bb)*(1.0*SCALE - ss) / (SCALE))*vv / SCALE;
	*r = CINT(rr);
	*g = CINT(gg);
	*b = CINT(bb);
	if (*r > 255) *r = 255;
	if (*g > 255) *g = 255;
	if (*b > 255) *b = 255;
}
