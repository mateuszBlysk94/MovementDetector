#include "FramesDifference.h"

Mat FramesDifference::GaussBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(minued, young, difference_helper);
	GaussianBlur(difference_helper, difference_help, Size(23, 23), 0, 0);
	thresholding(difference_help, difference_help, 5, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::MedianBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(minued, young, difference_helper);
	medianBlur(difference_helper, difference_help, 15);
	thresholding(difference_help, difference_help, 4, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::BoxBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(minued, young, difference_helper);
	boxFilter(difference_helper, difference_help, -1, Size(21, 21),
		Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help, difference_help, 4, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::DilatBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	dilate(difference_helper, difference_help, Mat(), Point(-1,-1), 
		1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	thresholding(difference_help, difference_help, 20, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::ErodeBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	erode(difference_helper, difference_help, Mat(), Point(-1,-1), 
		1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	thresholding(difference_help, difference_help, 6, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::SobelBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(minued, young, difference_helper);
	Sobel(difference_helper, difference_help, difference_helper.depth(), 
		1, 1, 3, 1, 0, BORDER_DEFAULT);
	thresholding(difference_help, difference_help, 16, 255, THRESH_BINARY);
	return difference_help;
}

Mat FramesDifference::CannyBlurScenario(Mat minued, Mat young)
{
	Mat difference_helper, difference_help;
	absdiff(minued, young, difference_helper);
	Canny(difference_helper, difference_help, 20, 100, 3, false);
	return difference_help;
}

Mat FramesDifference::ThresholdScenario(Mat minued, Mat young)
{
	Mat difference_helper;
	absdiff(minued, young, difference_helper);
	thresholding(difference_helper, difference_helper, 
		16, 255, THRESH_BINARY);
	return difference_helper;
}

Mat FramesDifference::GaussBlurScenarioDelayedWeightsAreas(Mat minued, 
	Mat young, Mat first, float firstTimeOfLive)
{
	Mat difference_helper, difference_help, difference_help2;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrix(difference_helper, first, young, firstTimeOfLive);
	GaussianBlur(difference_help, difference_help2, Size(23, 23), 0, 0);
	thresholding(difference_help2, difference_help2, 4, 255, THRESH_BINARY);
	return difference_help2;
}

Mat FramesDifference::BoxBlurScenarioDelayedWeightsAreas(Mat minued, Mat young, Mat first, float firstTimeOfLive)
{
	Mat difference_helper, difference_help, difference_help2, difference_help3;
	absdiff(minued, young, difference_helper);
	difference_help=WeightsMatrix(difference_helper, first, young, firstTimeOfLive);
	boxFilter(difference_help, difference_help2, -1, Size(21, 21), Point(-1,-1), true, BORDER_DEFAULT );
	thresholding(difference_help2, difference_help2, 4, 255, THRESH_BINARY);
	return difference_help2;
}
