#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

Mat effect_2_1(Mat input, Mat output)//加權平均平滑化
{
	int fil[9] = {
		1, 1, 1,
		1, 4, 1,
		1, 1, 1 };
	int x1, y1, x2, y2;
	int x, y;
	int xx, yy;
	int ff;
	Vec3b pix, col;

	x1 = 0;
	y1 = 0;
	x2 = input.cols - 1;
	y2 = input.rows - 1;
	int blue, green, red;
	for (y = y1; y<=y2; y++) {
		for (x = x1; x<=x2; x++) {
			ff = 0;
			blue = green = red = 0;
			for (yy = -1; yy <= 1; yy++) {
				for (xx = -1; xx <= 1; xx++) {
					if (y + yy < 0 || x + xx < 0) {
						pix.val[0] = 0;
						pix.val[1] = 0;
						pix.val[2] = 0;
					}
					else if (x + xx > x2 || y + yy > y2) {
						pix.val[0] = 0;
						pix.val[1] = 0;
						pix.val[2] = 0;
					}
					else pix = input.at<Vec3b>(y+yy, x+xx);

					blue += (pix.val[0] * fil[ff]);
					green += (pix.val[1] * fil[ff]);
					red +=  (pix.val[2] * fil[ff]);
					ff++;
				}
			}
			pix.val[0] = (blue/12);
			pix.val[1] = (green/12);
			pix.val[2] = (red/12);

			output.at<Vec3b>(y, x) = pix;
		}
	}
	return output;	
}

Mat effect_2_2(Mat input, Mat output) //銳利化
{
	int fil[9] = {
		0,-1, 0,
		-1, 5,-1,
		0,-1, 0 };
	int x1, y1, x2, y2;
	int x, y;
	int xx, yy;
	int ff;
	Vec3b pix, col;

	x1 = 0;
	y1 = 0;
	x2 = input.cols - 1;
	y2 = input.rows - 1;
	int blue, green, red;
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			ff = 0;
			blue = green = red = 0;
			for (yy = -1; yy <= 1; yy++) {
				for (xx = -1; xx <= 1; xx++) {
					if (y + yy < 0 || x + xx < 0) {
						pix.val[0] = 0;
						pix.val[1] = 0;
						pix.val[2] = 0;
					}
					else if (x + xx > x2 || y + yy > y2) {
						pix.val[0] = 0;
						pix.val[1] = 0;
						pix.val[2] = 0;
					}
					else pix = input.at<Vec3b>(y + yy, x + xx);

					blue += (pix.val[0] * fil[ff]);
					green += (pix.val[1] * fil[ff]);
					red += (pix.val[2] * fil[ff]);
					ff++;
				}
			}
			pix.val[0] = saturate_cast<uchar>(blue);
			pix.val[1] = saturate_cast<uchar>(green);
			pix.val[2] = saturate_cast<uchar>(red);

			output.at<Vec3b>(y, x) = pix;
		}
	}
	return output;
}

Mat effect_ano(Mat input, Mat output) //自創filter:取得邊緣
{
	int fil[9] = {
		0,-3, 0,
		-3, 0, 3,
		0, 3, 0 };
	int x1, y1, x2, y2;
	int x, y;
	int xx, yy;
	int ff;
	Vec3b pix, col;

	x1 = 0;
	y1 = 0;
	x2 = input.cols - 1;
	y2 = input.rows - 1;
	int blue, green, red;
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			ff = 0;
			blue = green = red = 0;
			for (yy = -1; yy <= 1; yy++) {
				for (xx = -1; xx <= 1; xx++) {
					if (y + yy < 0 || x + xx < 0) {
						pix.val[0] = 0;
						pix.val[1] = 0;
						pix.val[2] = 0;
					}
					else if (x + xx > x2 || y + yy > y2) {
						pix.val[0] = 0;
						pix.val[1] = 0;
						pix.val[2] = 0;
					}
					else pix = input.at<Vec3b>(y + yy, x + xx);

					blue += (pix.val[0] * fil[ff]);
					green += (pix.val[1] * fil[ff]);
					red += (pix.val[2] * fil[ff]);
					ff++;
				}
			}
			pix.val[0] = saturate_cast<uchar>(blue);
			pix.val[1] = saturate_cast<uchar>(green);
			pix.val[2] = saturate_cast<uchar>(red);

			output.at<Vec3b>(y, x) = pix;
		}
	}
	return output;
}

Mat effect_other(Mat input, Mat output) //自創filter:模糊
{
	int fil[25] = {
		1, 2, 3, 2, 1,
		2, 3, 4, 3, 2,
		3, 4, 5, 4, 3,
		2, 3, 4, 3, 2,
		1, 2, 3, 2, 1};
	int x1, y1, x2, y2;
	int x, y;
	int xx, yy;
	int ff;
	Vec3b pix, col;

	x1 = 0;
	y1 = 0;
	x2 = input.cols - 1;
	y2 = input.rows - 1;
	int blue, green, red;
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			ff = 0;
			blue = green = red = 0;
			for (yy = -2; yy <= 2; yy++) {
				for (xx = -2; xx <= 2; xx++) {
					if (y + yy < 0 || x + xx < 0) {
						pix.val[0] = 0;
						pix.val[1] = 0;
						pix.val[2] = 0;
					}
					else if (x + xx > x2 || y + yy > y2) {
						pix.val[0] = 0;
						pix.val[1] = 0;
						pix.val[2] = 0;
					}
					else pix = input.at<Vec3b>(y + yy, x + xx);

					blue += (pix.val[0] * fil[ff]);
					green += (pix.val[1] * fil[ff]);
					red += (pix.val[2] * fil[ff]);
					ff++;
				}
			}
			pix.val[0] = saturate_cast<uchar>(blue/65);
			pix.val[1] = saturate_cast<uchar>(green/65);
			pix.val[2] = saturate_cast<uchar>(red/65);

			output.at<Vec3b>(y, x) = pix;
		}
	}
	return output;
}


int main() {
	Mat src, res1, res2, res3, res4;
	src = imread("cap2_01a.bmp"); //test image
	//提示1: saturate_cast<uchar> => https://blog.csdn.net/piaoxuezhong/article/details/60570919
	//提示2: 程式會爆的話，確認讀取陣列時，有沒有超出圖像範圍
	res1 = src.clone();
	res2 = src.clone();
	res3 = src.clone();
	res4 = src.clone();

	imshow("src", src);
	imshow("smoothing", effect_2_1(src, res1));
	imshow("Sharpen", effect_2_2(src, res2));
	imshow("Contour: Self-made filter", effect_ano(src, res3));
	imshow("Blur: Self-made filter", effect_other(src, res4));

	waitKey(0);
	return 0;
}
