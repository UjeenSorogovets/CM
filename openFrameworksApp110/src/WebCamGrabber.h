#pragma one

#include "ofMain.h"
//#include "ofApp.h"
#include "ofxCv.h"
//#include <opencv2/ximgproc.hpp>
#include <chrono>


class WebCamGrabber
{
public:
	ofVideoGrabber vidGrabber;
	//ofPixels videoPix;
	ofImage videoImage;
	cv::Mat prevImg;
	cv::Mat backImg;
	//float stdFrames = 10.0;
	bool backStab = true;
	double backMean = 255;
	int camWidth = 640;
	int camHeight = 480;

	int globalW;
	int globalH;
	cv::RNG rng = cv::RNG(12345);

	std::chrono::time_point<std::chrono::system_clock> timer;
	std::chrono::duration<int, std::micro> gestureDur = 3s;

	std::function<void(int,int,int)> clickCallback;

	bool initialize(std::function<void(int, int, int)> clickCallback);

	void update();

	void draw(int w, int h, float a);
};