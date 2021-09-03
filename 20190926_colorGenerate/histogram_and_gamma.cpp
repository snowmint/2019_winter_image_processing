#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

Mat histogramTran(Mat input) { //histogram stretching
	int x, y, pval;
	Vec3b col, pix;
	int param1, param2;

	param1 = 300;
	param2 = -300;

	for (y = 0; y < input.rows; y++) {
		for (x = 0; x < input.cols; x++) {
			col = input.at<Vec3b>(y, x);
			uchar blue = col.val[0];//b
			uchar green = col.val[1];//g
			uchar red = col.val[2];//r
			
			pval = (blue + green + red) / 3;

			if (param1 > pval) param1 = pval;
			if (param2 < pval) param2 = pval;
		}
	}
	printf("p1 : %d ,", param1);
	printf("p2 : %d\n", param2);

	for (y = 0; y < input.rows; y++) {
		for (x = 0; x < input.cols; x++) {
			col = input.at<Vec3b>(y, x);
			uchar blue = col.val[0];
			uchar green = col.val[1];
			uchar red = col.val[2];
			pix.val[0] = saturate_cast<uchar>(255 * (blue - param1) / (param2 - param1));
			pix.val[1] = saturate_cast<uchar>(255 * (green - param1) / (param2 - param1));
			pix.val[2] = saturate_cast<uchar>(255 * (red- param1) / (param2 - param1));
			input.at<Vec3b>(y, x) = pix;
			
		}
	}
	return input;
}

Mat gammaTran(Mat input, double gamma) { //gamma correction
	
	int x, y;
	int i;
	int val;
	double a;
	int color_tbl[256];
	Vec3b col, pix;

	for (i = 0; i<256; i++) {
		a = (double)i / 255.0;
		color_tbl[i] = (int)(pow(a, gamma)*255.0);
	}

	for (y = 0; y<input.rows; y++) {
		for (x = 0; x<input.cols; x++) {
			col = input.at<Vec3b>(y, x);
			uchar blue = col.val[0];
			uchar green = col.val[1];
			uchar red = col.val[2];

			pix.val[0] = color_tbl[blue];
			pix.val[1] = color_tbl[green];
			pix.val[2] = color_tbl[red];

			input.at<Vec3b>(y, x) = pix;
		}
	}

	return input;
}


int main() {
	Mat src, res, res1;
	src = imread("cap2_01a.bmp");
	//防止溢出參考網址  https://blog.csdn.net/piaoxuezhong/article/details/60570919
	res = src.clone();
	res1 = src.clone();

	namedWindow("origin");
	imshow("origin", src);

	namedWindow("histogramTran");
	imshow("histogramTran", histogramTran(res));//(img)

	namedWindow("gammaTran");
	imshow("gammaTran", gammaTran(res1, 0.5));//(img, gamma)

	waitKey(0);
	return 0;
}
