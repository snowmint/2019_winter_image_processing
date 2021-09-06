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

void effect_5_11(Mat img, Mat outimg, int lv);
void effect_5_22(Mat img, Mat outimg, int* pat);
void makeHistogram(Mat img);

int RList[256] = { 0 };
int GList[256] = { 0 };
int BList[256] = { 0 };

int main() {
	Mat src;
	Mat res, res1, res2;

	//src = imread("Chrysanthemum.jpg");
	//src = imread("Parisyan.jpg");
	src = imread("HW12Test.png");
	res = src.clone();
	res1 = src.clone();
	res2 = src.clone();

	imshow("src", src);

	effect_5_11(src, res, 2); // 階調的削減
	imshow("Color Reduce: lv2", res);
	imwrite("Color_Reduce_lv2.png", res);

	effect_5_11(src, res, 5); // 階調的削減
	imshow("Color Reduce: lv5", res);
	imwrite("Color_Reduce_lv5.png", res);

	effect_5_11(src, res, 10); // 階調的削減
	imshow("Color Reduce: lv10", res);
	imwrite("Color_Reduce_lv10.png", res);

	// Bayer 型 
	int pat_bayer[16] = {
		1, 9, 3, 11,
		13, 5, 15, 7,
		4, 12, 2, 10,
		16,8,14,6
	};
	// 漩渦型
	int pat_shell[16] = {
		10, 9, 8, 7,
		11, 2, 1, 6,
		12, 3, 4, 5,
		13,14,15,16
	};

	effect_5_22(src, res1, pat_bayer); //由 RGB16 階調變換成 RGB2 階調 : Bayer型
	imshow("Dithering: Bayer", res1);
	imwrite("Dithering_Bayer.png", res1);

	effect_5_22(src, res2, pat_shell); //由 RGB16 階調變換成 RGB2 階調 : 漩渦型
	imshow("Dithering: Shell", res2);
	imwrite("Dithering_Shell.png", res2);

	makeHistogram(src);

	waitKey(0);
	return 0;
}

void effect_5_22(Mat img, Mat outimg, int* pat) {
	int x, y;
	int val;
	int rr, gg, bb;
	int adr;
	Vec3b col;

	for (y = 0; y<img.rows; y++) {
		for (x = 0; x<img.cols; x++) {
			if (y >= 0 && y < img.rows) {
				if (x >= 0 && x < img.cols) {
					col.val[2] = img.ptr<Vec3b>(y)[x].val[2];
					col.val[1] = img.ptr<Vec3b>(y)[x].val[1];
					col.val[0] = img.ptr<Vec3b>(y)[x].val[0];
					rr = col.val[2];
					gg = col.val[1];
					bb = col.val[0];
					adr = (x % 4) + (y % 4) * 4;
					rr /= 16;
					gg /= 16;
					bb /= 16;
					if (rr < pat[adr]) rr = 0;
					else rr = 255;
					if (gg < pat[adr]) gg = 0;
					else gg = 255;
					if (bb < pat[adr]) bb = 0;
					else bb = 255;

					col.val[2] = rr;
					col.val[1] = gg;
					col.val[0] = bb;

					outimg.ptr<Vec3b>(y)[x].val[2] = col.val[2];
					outimg.ptr<Vec3b>(y)[x].val[1] = col.val[1];
					outimg.ptr<Vec3b>(y)[x].val[0] = col.val[0];
				}
			}
		}
	}
}


void effect_5_11(Mat img, Mat outimg, int lv)
{
	int x, y;
	int val;
	int rr, gg, bb;
	int vv;
	int adr;
	Vec3b col;

	for (y = 0; y<img.rows; y++) {
		for (x = 0; x<img.cols; x++) {
			if (y >= 0 && y < img.rows) {
				if (x >= 0 && x < img.cols) {
					col.val[2] = img.ptr<Vec3b>(y)[x].val[2];
					col.val[1] = img.ptr<Vec3b>(y)[x].val[1];
					col.val[0] = img.ptr<Vec3b>(y)[x].val[0];
					rr = col.val[2];
					gg = col.val[1];
					bb = col.val[0];

					vv = rr * lv / 255;
					rr = vv * 255 / lv;

					vv = gg * lv / 255;
					gg = vv * 255 / lv;

					vv = bb * lv / 255;
					bb = vv * 255 / lv;

					col.val[2] = rr;
					col.val[1] = gg;
					col.val[0] = bb;

					outimg.ptr<Vec3b>(y)[x].val[2] = col.val[2];
					outimg.ptr<Vec3b>(y)[x].val[1] = col.val[1];
					outimg.ptr<Vec3b>(y)[x].val[0] = col.val[0];
				}
			}
		}
	}
}
void makeHistogram(Mat img)//參考5-13將直方圖繪製出來
{
	int hist_y = img.rows, hist_x = img.cols;
	int x, y;
	int i;
	Vec3b col;
	int rr, gg, bb;
	int acc_rr = 0, acc_gg = 0, acc_bb = 0;
	//繪製線段 => line(histImage, Point(x1, y1), Point(x2 ,y2), Scalar(255, 0, 0), 2, 8, 0);
	Mat histImage(256, 256*3, CV_8UC3, Scalar(0, 0, 0));//創建新的全黑img 繪製直方圖

	for (y = 0; y < hist_y; y++) {
		for (x = 0; x < hist_x; x++) {
			rr = img.ptr<Vec3b>(y)[x].val[2];
			gg = img.ptr<Vec3b>(y)[x].val[1];
			bb = img.ptr<Vec3b>(y)[x].val[0];
			RList[rr]++;
			GList[gg]++;	
			BList[bb]++;
		}
	}
	int max_rr = 0, max_gg = 0, max_bb = 0;
	for (int i = 0; i < 256; i++) {
		acc_rr += RList[i];
		acc_gg += GList[i];
		acc_bb += BList[i];
		if (max_rr < RList[i]) max_rr = RList[i];
		if (max_gg < GList[i]) max_gg = GList[i];
		if (max_bb < BList[i]) max_bb = BList[i];
	}
	float temp_rr = (float)max_rr / acc_rr;
	temp_rr *= 256;

	float temp_gg = (float)max_gg / acc_gg;
	temp_gg *= 256;

	float temp_bb = (float)max_bb / acc_bb;
	temp_bb *= 256;

	for (y = 1; y < 256*3; y++) {
		line(histImage, Point(y, (256 - (int)(( 256/ temp_rr)*(256 * (float)RList[y/3] / acc_rr)))), Point(y-1, (256 - (int)((256/ temp_rr)*(256 *(float)RList[(y-1)/3]/acc_rr)))), Scalar(0, 0, 255), 2, 8, 0);
		line(histImage, Point(y, (256 - (int)((256 / temp_gg)*(256 * (float)RList[y / 3] / acc_gg)))), Point(y - 1, (256 - (int)((256 / temp_gg)*(256 * (float)RList[(y - 1) / 3] / acc_gg)))), Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(y, (256 - (int)((256 / temp_bb)*(256 * (float)RList[y / 3] / acc_bb)))), Point(y - 1, (256 - (int)((256 / temp_bb)*(256 * (float)RList[(y - 1) / 3] / acc_bb)))), Scalar(255, 0, 0), 2, 8, 0);
	}

	imshow("【RGB直方圖-自製】", histImage);
	imwrite("【RGB直方圖-自製】.jpg", histImage);
}