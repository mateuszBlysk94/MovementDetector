#include "FramesDifference.h"

float FramesDifference::ElapsedTime=0;

FramesDifference::FramesDifference(Mat old, Mat young, Mat first,
			int scenario, string txtExport, bool ifWeights,
			 float firstTimeOfLive)
{
	Mat minued;
	minued=old;
	if (ifWeights)
		scenario+=100;

	switch (scenario)
	{
		//without weights
		case 1:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"GaussBlurScenario\n";
			difference=GaussBlurScenario(minued, young);
			break;
		case 2:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"MedianBlurScenario\n";
			difference=MedianBlurScenario(minued, young);
			break;
		case 3:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"BoxBlurScenario\n";
			difference=BoxBlurScenario(minued, young);
			break;
		case 4:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"DilatBlurScenario\n";
			difference=DilatBlurScenario(minued, young);
			break;
		case 5:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"SobelBlurScenario\n";
			difference=SobelBlurScenario(minued, young);
			break;
		case 6:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"ErodeBlurScenario\n";
			difference=ErodeBlurScenario(minued, young);
			break;
		case 7:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"CannyBlurScenario\n";
			difference=CannyBlurScenario(minued, young);
			break;
		case 8:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"ThresholdScenario\n";
			difference=ThresholdScenario(minued, young);
			break;
			
		//with weights
		case 101:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"GaussBlurScenarioDelayedWeightsAreas\n";
			difference=GaussBlurScenarioDelayedWeightsAreas(minued, young,
										first, firstTimeOfLive);
			break;
		case 102:
			if (FramesDifference::ElapsedTime==0) 
				cout<<"BoxBlurScenarioDelayedWeightsAreas\n";
			difference=BoxBlurScenarioDelayedWeightsAreas(minued, young, 
										first, firstTimeOfLive);
			break;
	}
	
	movingPixels(txtExport);
}

//the value of threshold is equal to the short number
void FramesDifference::thresholding(Mat inImage, Mat outImage, short thres, 
											double maxval, int type)
{
	threshold(inImage, outImage, thres, maxval, type);
}

//exporting data
void FramesDifference::movingPixels(string txtExport)
{
	int nonZero=(countNonZero(difference));
	int numberOfPixels=difference.cols*difference.rows;
	//export data to the txt file
	const char* txtExp = txtExport.c_str();
	ofstream plik(txtExp, ios::app);
	plik << ElapsedTime <<"\t"<< ((float)nonZero/numberOfPixels) << endl; 
	plik.close();
}

//multipling pixels of frame with proper weight from weight matrix
Mat multiply (Mat frame, float* weights, short yDist, short xDist, 
	float piksOnYDist, float piksOnXDist)
{
	for (int i=0; i<yDist; i++)
	{
		for (int j=0; j<xDist; j++)
		{
			for (int l=0; l<piksOnXDist; l++)
			{
				for (int k=0; k<piksOnYDist; k++)
				{
					frame.at<uchar>(k+i*piksOnYDist, 
						l+j*piksOnXDist)*=weights[i*xDist+j];
				}
			}
		}
	}

	return frame;
}
