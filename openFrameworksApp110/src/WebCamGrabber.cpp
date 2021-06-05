#include "WebCamGrabber.h"
#include "opencv2/ximgproc.hpp"

bool WebCamGrabber::initialize(std::function<void(int, int, int)> clickCallback)
{
	this->clickCallback = clickCallback;
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
		cv::flip(in, in, 1);
		cv::Mat out, gray;
		cv::cvtColor(in, gray, cv::COLOR_BGR2GRAY);
		if (!backImg.empty())
		{

			if (backStab)
			{
				//cout << backMean << endl;
				string text = "Don't move to identify background [" + std::to_string((int)(255-backMean) * 100 / 255) + "\%]";
				cv::Size textSize= cv::getTextSize(text, cv::FONT_HERSHEY_PLAIN,1.5, 1,nullptr);
				cv::Point2i textCoord;
				textCoord.x = camWidth / 2 - textSize.width / 2;
				textCoord.y = camHeight / 2 - textSize.height / 2;
				cv::putText(in, text, textCoord, cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 0, 0,255), 3);
				cv::putText(in, text, textCoord, cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar( backMean, 255, backMean,255), 2);
				cv::accumulateWeighted(gray, backImg, 0.02);
			}
			cv::Mat back;
			cv::Mat diff;
			cv::convertScaleAbs(backImg, back);
			cv::absdiff(back, gray, back);
			//cv::GaussianBlur(diff, diff, cv::Size(7, 7), 0);
			cv::threshold(back, diff, 30, 255, cv::THRESH_BINARY);
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
			cv::Mat mask;

			cv::morphologyEx(diff, diff, cv::MORPH_ERODE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)));
			cv::morphologyEx(diff, diff, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));
			std::vector<cv::Mat> channels;
			cv::split(in, channels);
			if (!backStab)
			{
				cv::morphologyEx(diff, mask, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)));
				channels.push_back(mask);
			}
			else
			{
				//channels.push_back(cv::Mat::ones(in.size(), CV_8UC1));
			}
			cv::merge(channels, out);
			//cv::morphologyEx(diff, diff, cv::MORPH_ERODE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)));
			//cv::copyMakeBorder(diff, diff, -50, -50, -50, -50, cv::BORDER_CONSTANT, cv::Scalar(0));

			cv::Rect border(cv::Point(0, 0), diff.size());
			int thickness = 50;

			cv::rectangle(diff, border, cv::Scalar(0), thickness);

			vector<vector<cv::Point> > contours;
			vector<cv::Vec4i> hierarchy;
			findContours(diff, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
			//cv::Mat drawing = cv::Mat::zeros(in.size(), CV_8UC3);
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
			cv::Point2i mouse = cv::Point(0,0);
			bool gesturePress = false;
			if (backStab)
			{
				color = cv::Scalar(255, 0, 0,255);
			}
			else
			{
				if (max_size > 4000)
				{
					gesturePress = true;
					color = cv::Scalar(0, 0, 255, 255);
					auto rect = cv::minAreaRect(contours[max_i]);/*
					cv::Point2f points[4];
					rect.points(points);

					cv::circle(drawing, points[0], 2, color, 2);
					cv::circle(drawing, points[1], 2, color, 2);
					cv::circle(drawing, points[2], 2, color, 2);
					cv::circle(drawing, points[3], 2, color, 2);*/
					/*
					mouse.x = (points[0].x + points[1].x) / 2;
					mouse.y = (points[0].y + points[1].y) / 2;*/
					mouse = rect.center;
					cv::circle(out , mouse, 10, color, 2);
				}
				else
				{
					color = cv::Scalar(0, 255, 0,255);
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
					//auto rect = cv::minAreaRect(contours[max_i]);
					clickCallback((1.0*mouse.x*globalW/camWidth), (1.0*mouse.y*globalH / camHeight), 99);

					color = cv::Scalar(255, 255, 255, 255);
					cv::circle(out, cv::minAreaRect(contours[max_i]).center, 6, color, 4);
				}
			}

			//drawContours(drawing, contours, (int)max_i, color, 2, cv::LINE_8, hierarchy, 0);
			//cv::ximgproc::thinning(diff, diff, cv::ximgproc::THINNING_GUOHALL);

			//cout << backMean << endl;
			//cv::adaptiveThreshold(diff, out, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 7, 10);
			//cv::Canny(out, out, 50, 200);
			//cv::addWeighted(drawing, 0.5, out, 0.5, 0, out);
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
	videoImage.draw(0, 0, w, h);
	globalH = h;
	globalW = w;
	ofDisableAlphaBlending();
}