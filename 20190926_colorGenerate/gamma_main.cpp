#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//gamma < 1 light
//gamma > 1 dark

void MyGammaCorrection(Mat& src, Mat& dst, float fGamma)
{

	// build look up table  
	unsigned char lut[256];
	for (int i = 0; i < 256; i++)
	{
		lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
	}

	dst = src.clone();
	const int channels = dst.channels();
	switch (channels)
	{
	case 1:   //灰度圖的情況
	{

		MatIterator_<uchar> it, end;
		for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
			//*it = pow((float)(((*it))/255.0), fGamma) * 255.0;  
			*it = lut[(*it)];

		break;
	}
	case 3:  //彩色圖的情況
	{

		MatIterator_<Vec3b> it, end;
		for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++)
		{
			//(*it)[0] = pow((float)(((*it)[0])/255.0), fGamma) * 255.0;  
			//(*it)[1] = pow((float)(((*it)[1])/255.0), fGamma) * 255.0;  
			//(*it)[2] = pow((float)(((*it)[2])/255.0), fGamma) * 255.0;  
			(*it)[0] = lut[((*it)[0])];
			(*it)[1] = lut[((*it)[1])];
			(*it)[2] = lut[((*it)[2])];
		}

		break;

	}
	}
}

int main()
{
	Mat image = imread("gamma_pending.jpg");
	if (image.empty())
	{
		cout << "Error: Could not load image" << endl;
		return 0;
	}

	Mat dst;
	float fGamma = 1 / 2.0;
	MyGammaCorrection(image, dst, fGamma);

	imshow("Source Image", image);
	imshow("Dst", dst);

	waitKey();

	return 0;
}