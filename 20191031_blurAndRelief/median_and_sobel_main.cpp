#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

#define PAI   3.1415926
#define WSIZE 27

int median(int *sort);
void effect_3_7(Mat img, Mat outimg);
void effect_3_10(Mat img, Mat outimg);

int main() {
	Mat src, res1, res2;
	//src = imread("cap3_02a.bmp"); //test image
	src = imread("cap3_02a.bmp"); //test image
	imshow("src", src);

	
	res1 = src.clone();
	res2 = src.clone();

	effect_3_7(src, res1);
	effect_3_10(src, res2);

	imshow("sobel_keep_edge_smoothing", res1);
	imshow("median filtering", res2);

	imwrite("sobel_keep_edge_smoothing.bmp", res1);
	imwrite("median_filtering.bmp", res2);

	waitKey(0);
	return 0;
}

void effect_3_7(Mat img, Mat outimg)//sobel邊緣保存平滑化
{
	int val;
	int x, y;
	int xx, yy;
	int i;
	int hh;
	int du, dd, dl, dr;
	int endn;
	int c1, c2;
	int rrx, ggx, bbx;
	int rry, ggy, bby;
	int rr, gg, bb;
	Vec3b col, ncol;
	int *sobel;
	int sadr;
	int rate, vh;
	int fil[5 * 5], para, fsam;
	int sobel1[9] = {
		1, 0,-1,
		2, 0,-2,
		1, 0,-1 };
	int sobel2[9] = {
		1, 2, 1,
		0, 0, 0,
		-1,-2,-1 };
	int x1, y1, x2, y2;

	x1 = 0;
	y1 = 0;
	x2 = img.cols - 1;
	y2 = img.rows - 1;

	int widthLimit = img.channels() * img.cols;

	for (y = 0; y <= y2; y++) {
		//Vec3b *data2 = outimg.ptr<Vec3b>(y);
		for (x = 0; x <= widthLimit; x++) {
			rrx = ggx = bbx = 0;
			rry = ggy = bby = 0;
			sadr = 0;
			for (yy = -1; yy <= 1; yy++) {
				for (xx = -1; xx <= 1; xx++) {
					if (y + yy >= 0 && y + yy < img.rows) {
						if (x + xx >= 0 && x + xx < img.cols) {
							//Vec3b *data = img.ptr<Vec3b>(y + yy);
							rrx += img.ptr<Vec3b>(y + yy)[x + xx].val[2] * sobel1[sadr];
							ggx += img.ptr<Vec3b>(y + yy)[x + xx].val[1] * sobel1[sadr];
							bbx += img.ptr<Vec3b>(y + yy)[x + xx].val[0] * sobel1[sadr];
							rry += img.ptr<Vec3b>(y + yy)[x + xx].val[2] * sobel2[sadr];
							ggy += img.ptr<Vec3b>(y + yy)[x + xx].val[1] * sobel2[sadr];
							bby += img.ptr<Vec3b>(y + yy)[x + xx].val[0] * sobel2[sadr];
						}
					}
					sadr++;
				}
			}
			para = (int)(sqrt((double)(rrx*rrx + rry * rry)) / 8.0);
			para = 30 - para;
			if (para<0) para = 0;
			for (i = 0; i<25; i++) {
				fil[i] = para;
			}
			fil[2 * 5 + 2] = 30;
			rr = gg = bb = 0;
			fsam = 0;
			sadr = 0;
			for (yy = -2; yy<3; yy++) {
				for (xx = -2; xx<3; xx++) {
					if (y + yy >= 0 && y + yy < img.rows) {
						if (x + xx >= 0 && x + xx < img.cols) {
							//Vec3b *data = img.ptr<Vec3b>(y + yy);
							rr += img.ptr<Vec3b>(y + yy)[x + xx].val[2] * fil[sadr];
							gg += img.ptr<Vec3b>(y + yy)[x + xx].val[1] * fil[sadr];
							bb += img.ptr<Vec3b>(y + yy)[x + xx].val[0] * fil[sadr];
							fsam += fil[sadr];
						}
					}
					sadr++;
				}
			}
			//cout << fsam << endl;
			if (fsam != 0) {
				outimg.ptr<Vec3b>(y)[x].val[2] = rr / fsam;
				outimg.ptr<Vec3b>(y)[x].val[1] = gg / fsam;
				outimg.ptr<Vec3b>(y)[x].val[0] = bb / fsam;
			}
		}
	}
}
void effect_3_10(Mat img, Mat outimg) //中值法 median filtering
{
	int val;
	int x, y;
	int xx, yy;
	int rr, gg, bb;
	int endn;
	Vec3b col, col2, pix;
	int rd;
	int sortr[9] = { 0 }, sortg[9] = { 0 }, sortb[9] = { 0 };
	int x1, y1, x2, y2;
	x1 = 0;
	y1 = 0;
	x2 = img.cols - 1;
	y2 = img.rows - 1;

	int widthLimit = img.channels() * img.cols;

	for (y = 0; y <= y2; y++) {
		//Vec3b *data2 = outimg.ptr<Vec3b>(y);
		for (x = 0; x <= x2; x++) {
			rr = gg = bb = 0;
			rd = 0;
			for (yy = -1; yy <= 1; yy++) {
				for (xx = -1; xx <= 1; xx++) {
					if (y + yy >= 0 && y + yy < img.rows) {
						if (x + xx >= 0 && x + xx < img.cols) {
							//Vec3b *data = img.ptr<Vec3b>(y + yy);
							pix = img.at<Vec3b>(y + yy, x + xx);
							sortr[rd] = pix.val[2];
							sortg[rd] = pix.val[1];
							sortb[rd] = pix.val[0];

							/*col2.val[2] = img.ptr<Vec3b>(y + yy)[x + xx].val[2];
							col2.val[1] = img.ptr<Vec3b>(y + yy)[x + xx].val[1];
							col2.val[0] = img.ptr<Vec3b>(y + yy)[x + xx].val[0];
							sortr[rd] = col2.val[2];
							sortg[rd] = col2.val[1];
							sortb[rd] = col2.val[0];*/
							//sortr[rd] = img.ptr<Vec3b>(y + yy)[x + xx].val[2];
							//sortg[rd] = img.ptr<Vec3b>(y + yy)[x + xx].val[1];
							//sortb[rd] = img.ptr<Vec3b>(y + yy)[x + xx].val[0];
						}
					}
					rd++;
				}
			}
			col.val[2] = median(sortr);
			col.val[1] = median(sortg);
			col.val[0] = median(sortb);
			outimg.at<Vec3b>(y, x) = col;
		}
	}
}
int median(int *sort)
{
	int i, j, n;
	int max;

	for (i = 0; i < 5; i++) {
		for (j = 1, n = 0, max = sort[0]; j<9; j++) {
			if (max<sort[j]) {
				max = sort[j];
				n = j;
			}
		}
		sort[n] = -1;
	}
	return max;
}
