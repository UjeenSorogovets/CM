#include "WebCamGrabber.h"
#include "opencv2/ximgproc.hpp"

bool WebCamGrabber::initialize()
{
	//this->app = app;
	//playPausePlayer(nullptr);
	//get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	cout << "[DEVICE LIST]" << endl;
	for (size_t i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			//log the device
			cout << devices[i].id << ": " << devices[i].deviceName << endl;
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
		OF_IMAGE_GRAYSCALE);
	ofSetVerticalSync(true);
	return true;
}

void WebCamGrabber::update()
{
	vidGrabber.update();

	if (vidGrabber.isFrameNew()) {

		cv::Mat in = ofxCv::toCv(vidGrabber);
		cv::Mat out, gray;
		cv::cvtColor(in, gray, cv::COLOR_BGR2GRAY);
		if (!backImg.empty())
		{

			if (backStab)
			{
				cout << backMean << endl;
				cv::accumulateWeighted(gray, backImg, 0.02);
			}
			cv::Mat diff;
			cv::convertScaleAbs(backImg, diff);
			cv::absdiff(diff, gray, diff);
			//cv::GaussianBlur(diff, diff, cv::Size(7, 7), 0);
			cv::threshold(diff, diff, 30, 255, cv::THRESH_BINARY);
			//diff.copyTo(out);
			backMean = cv::mean(diff)[0];

			if (backMean > 0.5 * 255)
			{
				backStab = true;
			}
			if (backMean < 0.01 * 255)
			{
				backStab = false;
			}


			cv::morphologyEx(diff, diff, cv::MORPH_ERODE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)));
			cv::morphologyEx(diff, diff, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));
			//cv::morphologyEx(diff, diff, cv::MORPH_ERODE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)));
			//cv::copyMakeBorder(diff, diff, -50, -50, -50, -50, cv::BORDER_CONSTANT, cv::Scalar(0));

			cv::Rect border(cv::Point(0, 0), diff.size());
			int thickness = 50;

			cv::rectangle(diff, border, cv::Scalar(0), thickness);

			vector<vector<cv::Point> > contours;
			vector<cv::Vec4i> hierarchy;
			findContours(diff, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
			cv::Mat drawing = cv::Mat::zeros(in.size(), CV_8UC3);
			size_t max_size = 0;
			size_t max_i = 0;
			for (size_t i = 0; i < contours.size(); i++)
			{
				if (contours[i].size() > max_size)
				{
					max_size = cv::contourArea(contours[i]);
					max_i = i;
				}
			}
			cv::Scalar color;
			bool gesturePress = false;
			if (backStab)
			{
				color = cv::Scalar(255, 0, 0);
			}
			else
			{
				if (max_size > 4000)
				{
					color = cv::Scalar(0, 0, 255);
				}
				else
				{
					gesturePress = true;
					color = cv::Scalar(0, 255, 0);
				}
				//cout << "AREA: " << max_size << endl;
			}

			if (!gesturePress)
			{
				timer = std::chrono::system_clock::now();
			}
			else
			{
				if (std::chrono::system_clock::now() - timer > gestureDur)
				{
					timer = std::chrono::system_clock::now();
					//auto M = cv::moments(contours[max_i]);
					//app->mousePressed(M.m10/M.m00, M.m01 / M.m00, 0);
				}
			}

			drawContours(drawing, contours, (int)max_i, color, 2, cv::LINE_8, hierarchy, 0);

			//cv::ximgproc::thinning(diff, diff, cv::ximgproc::THINNING_GUOHALL);

			//cout << backMean << endl;
			//cv::adaptiveThreshold(diff, out, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 7, 10);
			//cv::Canny(out, out, 50, 200);
			cv::addWeighted(drawing, 0.5, in, 0.5, 0, out);
			ofxCv::toOf(out, videoImage);
			videoImage.update();
		}
		else
		{
			backImg = cv::Mat::zeros(gray.size(), CV_32F);
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

void WebCamGrabber::draw(int w, int h, float a)
{
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, a);
	videoImage.draw(w, 0, -w, h);
	ofDisableAlphaBlending();
}