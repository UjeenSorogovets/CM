#pragma one
#include "ofMain.h"
#include "ofxCv.h"

class WebCamGrabber
{
public:
	ofVideoGrabber vidGrabber;
	ofPixels videoPix;
	ofImage videoImage;
	cv::Mat prevImg;
	cv::Mat backImg;
	int stdFrames = 10;

	int camWidth = 640;
	int camHeight = 480;

	bool initialize()
	{

		//get back a list of devices.
		vector<ofVideoDevice> devices = vidGrabber.listDevices();
		cout << "[DEVICE LIST]" << endl;
		for (size_t i = 0; i < devices.size(); i++) {
			if (devices[i].bAvailable) {
				//log the device
				cout << devices[i].id << ": " << devices[i].deviceName <<endl;
			}
			else {
				//log the device and note it as unavailable
				cout << devices[i].id << ": " << devices[i].deviceName << " - unavailable " << endl;
			}

		}
		vidGrabber.setDeviceID(0);
		vidGrabber.setDesiredFrameRate(30);
		vidGrabber.initGrabber(camWidth, camHeight);
		//vidGrabber.videoSettings();
		if (!vidGrabber.isInitialized())
		{
			return 0;
		}
		//videoPix.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(),
		//	vidGrabber.getPixelFormat());
		videoImage.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(),
			OF_IMAGE_COLOR);
		ofSetVerticalSync(true);
		return true;
	}

	void update()
	{
		vidGrabber.update();

		if (vidGrabber.isFrameNew()) {

			cv::Mat in = ofxCv::toCv(vidGrabber);
			cv::Mat out, gray;
			cv::cvtColor(in, gray, cv::COLOR_BGR2GRAY);
			if (!backImg.empty())
			{
				cv::accumulateWeighted(gray, backImg, 0.02);
				cv::Mat diff;
				cv::convertScaleAbs(backImg, diff);
				cv::absdiff(diff, gray, diff);
				//cv::Canny(diff, out, 50, 200);
				ofxCv::toOf(diff, videoImage);
				videoImage.update();
			}
			else
			{
				backImg = cv::Mat::zeros(gray.size(),CV_32F);
				ofxCv::toOf(gray, videoImage);
				videoImage.update();
			}
			prevImg = gray;
			//ofPixels & pixels = vidGrabber.getPixels();
			//for (size_t i = 0; i < pixels.size(); i++) {
			//	//invert the color of the pixel
			//	videoPix[i] = 255 - pixels[i];
			//}
			//load the inverted pixels
			//videoTexture.loadData(videoPix);
		}
	}

	void draw(int w, int h, float a)
	{
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, a);
		videoImage.draw(w, 0, -w, h);
		ofDisableAlphaBlending();
	}
};