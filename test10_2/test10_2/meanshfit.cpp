#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help(char** argv)
{
	cout << "\nDemonstrate mean-shift based color segmentation in spatial pyramid.\n"
		<< "Call:\n   " << argv[0] << " image\n"
		<< "This program allows you to set the spatial and color radius\n"
		<< "of the mean shift window as well as the number of pyramid reduction levels explored\n"
		<< endl;
}

//This colors the segmentations
//Scalar：all(n) 的含义是令里面的元素全为n
/*
typedef struct Scalar
{
double val[4];
}Scalar;
可以存放4组数据，val[0],val[1],val[2]可以表示3通道的数据
*/
static void floodFillPostprocess(Mat& img, const Scalar& colorDiff = Scalar::all(1))
{
	//CV_Assert断言判断
	CV_Assert(!img.empty());
	//产生随机数的
	RNG rng = theRNG();
	Mat mask(img.rows + 2, img.cols + 2, CV_8UC1, Scalar::all(0));
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			if (mask.at<uchar>(y + 1, x + 1) == 0)
			{
				//生成（0，256）之间的一个数
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(img, mask, Point(x, y), newVal, 0, colorDiff, colorDiff);
			}
		}
	}
}

string winName = "meanshift";
int spatialRad, colorRad, maxPyrLevel;
Mat img, res;

static void meanShiftSegmentation(int, void*)
{
	cout << "spatialRad=" << spatialRad << "; "
		<< "colorRad=" << colorRad << "; "
		<< "maxPyrLevel=" << maxPyrLevel << endl;
	pyrMeanShiftFiltering(img, res, spatialRad, colorRad, maxPyrLevel);
	floodFillPostprocess(res, Scalar::all(2));
	imshow(winName, res);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		help(argv);
		return -1;
	}

	img = imread(argv[1]);
	if (img.empty())
		return -1;

	spatialRad = 10;
	colorRad = 10;
	maxPyrLevel = 1;

	namedWindow(winName, WINDOW_AUTOSIZE);

	createTrackbar("spatialRad", winName, &spatialRad, 80, meanShiftSegmentation);
	createTrackbar("colorRad", winName, &colorRad, 60, meanShiftSegmentation);
	createTrackbar("maxPyrLevel", winName, &maxPyrLevel, 5, meanShiftSegmentation);

	meanShiftSegmentation(0, 0);
	waitKey();
	return 0;
}
