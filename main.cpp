/* Author: Mateusz Osiński
 * The program is a part of engineering work:
 * "Movement detection algorithm in the apnea monitor"
 * In this program same movement detection algorithms are presented
 */
#include <time.h> 
#include <cstdlib>
 
#include "FramesDifference.h"

//this function gives an arm to the video. 
//the function returns fps 
//if we choose a live transmition, fps is counted
float captureArm (int choice, string nameOfFile, VideoCapture *arm)
{
	float fps;
	if (choice==1)
	{
		*arm=VideoCapture(nameOfFile);
		fps=arm->get(CV_CAP_PROP_FPS);
	}
	else
	{
		*arm=VideoCapture(0); //0- default camera
		//start time
		time_t start, end;
		Mat lostFrames;
		time(&start);
		//grab 30 frames
		for(int i = 0; i < 30; i++)
		{
			(*arm) >> lostFrames;
		}
		//end Time
		time(&end);
		//total time
		double seconds = difftime (end, start);
		// Calculate frames per second
		cout<<seconds<<endl;
		fps  = 30 / seconds;
	}
	return fps; 
}
 

int main ()
{
	//capture the video
	cout<<"Analiza na podstawie nagranego filmu: 1, na żywo: 0 : ";
	short choice;
	cin>>choice;
	
	string nameOfFile;
	if (choice==1)
	{
		cout<<"Podaj nazwę pliku z filmem do odtworzenia: ";
		cin>>nameOfFile;
	}
	
	VideoCapture arm;
	float fps; //frames per second
	
	fps=captureArm(choice, nameOfFile, &arm);

	if (!arm.isOpened())
	{
		cout<<"brak pliku lub niewłaściwe nawiązanie połączenia z kamerą\n";
		return -1;
	}
	
	float firstTimeOfLive; //time of live of the firstFrame
	cout<<"Analiza zostanie przeprowadzona na podstawie"
		" odejmowania od siebie kolejnych klatek filmu."
		" Co druga klatka jest pomijana.\n";
	
	cout<<"Czy uwzględnić współczynniki wagowe (uwzglądnienie historii)"
			" 1- tak, 0- nie : ";
	bool ifWeights;
	cin>>ifWeights;	
	if (ifWeights)
		cout<<"\nWybierz scenariusz:\nFiltr Gaussa z wagami: 1"
			"\nFiltr pudełkowy z wagami: 2: \n";
	else
		cout<<"\nWybierz scenariusz:\nFiltr Gaussa: 1 \nFiltr medianowy: " 
			"2\nFiltr pudełkowy: 3: \nRozszerzenie: 4\nFiltr Sobela: 5"
			" \nErozja: 6: \nFiltr Canny'ego: 7 \nProgowanie: 8 \n";
	int scenario;
	cin>>scenario;
	
	if (ifWeights)
	{
		cout<<"Co ile sekund modyfikować współczynniki wagowe? (czas martwy): ";
		cin>>firstTimeOfLive;
	}
	short ifFirst=ifWeights;
	
	cout<<"Podaj nazwę pliku do zapisu danych tekstowych (bez rozszerzenia): ";
	string txtExport;
	cin>>txtExport;
	txtExport+=".txt";
	
	//read the first frame
	Mat olderFrame, youngerFrame, olderFrameConv, youngerFrameConv;
	Mat firstFrameConv;
	arm>>olderFrame;
	
	//conversion from RGB to gray color space
	cvtColor(olderFrame, olderFrameConv, CV_RGB2GRAY); 
	
	olderFrameConv.copyTo(firstFrameConv);
	cout << "klatki" << fps << endl;
	//distance between frames in ms
	double framesDistance = 1000 / fps;
	//creating a window
	namedWindow("window", WINDOW_AUTOSIZE);

	//a loop in which the video is analysing and printing in window
	while (arm.grab())
	{
		//getting next frame- skip one frame
		if(!arm.grab()) break;
		arm.retrieve(youngerFrame);
		//conversion from RGB to Gray scale
		cvtColor(youngerFrame, youngerFrameConv, CV_RGB2GRAY);
		//object of the new class- difference between following frames
		FramesDifference difference=FramesDifference
			(olderFrameConv, youngerFrameConv, firstFrameConv, 
				scenario, txtExport, ifWeights, firstTimeOfLive);
		youngerFrameConv.copyTo(olderFrameConv);
		imshow("window", difference.getDifference());

		//ElapsedTime- multiply by 2 because of skipping frames
		FramesDifference::ElapsedTime+=(2*framesDistance/1000); 
		if (ifFirst==1)
		{
			static int nr=1;
			if(FramesDifference::ElapsedTime>=firstTimeOfLive*nr)
				{
					youngerFrameConv.copyTo(firstFrameConv);
					nr++;
				}
		}
		cout<<"czas: "<<FramesDifference::ElapsedTime<<endl;
		//waiting 2*framesDistance because of skipping frames
		int stop = waitKey(2*framesDistance);
		//breaking the loop with any key
		if (stop+1) break; 
	}
	cout<<"Program kończy działanie...\n";
	return 0;
	 
}
