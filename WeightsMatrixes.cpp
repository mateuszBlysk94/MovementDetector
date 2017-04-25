#include "FramesDifference.h"

//we are summing values of pixels in small areas
//the bigger sum, the bigger area value
//The Weights matrix is updating after every firstTimeOfLive
Mat FramesDifference::WeightsMatrix(Mat frame, Mat first, Mat young, float firstTimeOfLive)
{
	Mat difference;
	absdiff(first, young, difference);
	const short xDist=24, yDist=12;
	float piksOnXDist=(float)difference.cols/xDist;
	float piksOnYDist=(float)difference.rows/yDist;
	const int amount=xDist*yDist;
	static int nr=1;
	//initialization of the matrix
	if (ElapsedTime==0)
	{
		Weights=new float[amount];
		for (int i=0; i<amount; i++)
			Weights[i]=1.0;
	}
	//updating the matrix
	else
	{
		static float max=0;
		float tab[amount]={0};
		static float tab2[amount];
		for (int i=0; i<yDist; i++)
		{
			for (int j=0; j<xDist; j++)
			{
				for (int k=0; k<piksOnYDist; k++)
				{
					for (int l=0; l<piksOnXDist; l++)
					{
						tab[i*xDist+j]+=(float)difference.at<uchar>(k+i*piksOnYDist,l+j*piksOnXDist);
					}
				}
			}
		}
		for (int i=0; i<yDist; i++)
		{
			for (int j=0; j<xDist; j++)
			{
				if (tab[i*xDist+j]>tab2[i*xDist+j])
				{
					tab2[i*xDist+j]=tab[i*xDist+j];
					if (tab2[i*xDist+j]>=max) max=tab2[i*xDist+j];
				}
			}
		}
		if(FramesDifference::ElapsedTime>=firstTimeOfLive*nr)
		{
			float WeightsMax=0;
			for (int i=0; i<(amount); i++)
			{
				Weights[i]*=3;
				Weights[i]+=(tab2[i]/max);
				Weights[i]/=4;
				tab2[i]=0;
				if (Weights[i]>WeightsMax)
					WeightsMax=Weights[i];
			}
			for (int i=0; i<(amount); i++)
			{
				Weights[i]/=WeightsMax;
			}
			nr++;
			max=0;
			
		}
	}
	//multiply the frame with weight matrix
	return multiply(frame, Weights, yDist, xDist, piksOnYDist, piksOnXDist);
}

