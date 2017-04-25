#ifndef FramesDifference_h
#define FramesDifference_h

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;

//in this class the result of frames substraction is stored - difference
class FramesDifference
{
	Mat difference;
	float * Weights;
	
	public:
	//elapsed time from first substraction
	static float ElapsedTime;
	FramesDifference(Mat old, Mat young, Mat first, int scenario,
			string txtExport, bool ifWeights, float firstTimeOfLive);
	Mat getDifference(void) {return difference;};
	void thresholding(Mat, Mat, short, double, int); 
	void movingPixels(string txtExport);
	//scenarios without weights
	Mat GaussBlurScenario(Mat, Mat);
	Mat MedianBlurScenario(Mat, Mat);
	Mat BoxBlurScenario(Mat, Mat);
	Mat DilatBlurScenario(Mat, Mat);
	Mat SobelBlurScenario(Mat, Mat);
	Mat ErodeBlurScenario(Mat, Mat);
	Mat CannyBlurScenario(Mat, Mat);
	Mat ThresholdScenario(Mat, Mat);
	//scenarios with weights
	Mat GaussBlurScenarioDelayedWeightsAreas(Mat, Mat, Mat, float);
	Mat BoxBlurScenarioDelayedWeightsAreas(Mat, Mat, Mat, float);
	
	Mat WeightsMatrix(Mat, Mat, Mat, float);
	
};

Mat multiply (Mat, float*, short, short, float, float);

#endif
