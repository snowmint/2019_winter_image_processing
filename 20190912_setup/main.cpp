#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat img = imread("learn.jpg");
	namedWindow("test");
	imshow("test", img);
	waitKey(0);

	return 0;
}
