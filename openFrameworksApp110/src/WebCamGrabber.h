#pragma one

#include "ofMain.h"
#include "ofxCv.h"
#include <opencv2/ximgproc.hpp>
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
	cv::RNG rng = cv::RNG(12345);

	std::chrono::time_point<std::chrono::system_clock> timer;
	std::chrono::duration<int, std::micro> gestureDur = 1s;

	//ofApp *app;

	bool initialize();

	void update();

	void draw(int w, int h, float a);
};