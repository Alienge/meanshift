#pragma once
#include "opencv2/highgui/highgui.hpp"

#include "opencv2/core/core.hpp"

#include "opencv2/imgproc/imgproc.hpp"
#include"cvpymeanshfit.cpp"
void pyrMeanShiftFiltering(InputArray _src, OutputArray _dst, int **&a,
	double sp, double sr, int maxLevel
	);