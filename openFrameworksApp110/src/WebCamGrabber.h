#pragma one
#include "ofMain.h"

class WebCamGrabber
{
public:
	ofVideoGrabber vidGrabber;
	ofPixels videoPix;
	ofTexture videoTexture;

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
		videoPix.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(),
			vidGrabber.getPixelFormat());
		videoTexture.allocate(videoPix);
		ofSetVerticalSync(true);
		return true;
	}

	void update()
	{
		vidGrabber.update();

		if (vidGrabber.isFrameNew()) {
			ofPixels & pixels = vidGrabber.getPixels();
			for (size_t i = 0; i < pixels.size(); i++) {
				//invert the color of the pixel
				videoPix[i] = 255 - pixels[i];
			}
			//load the inverted pixels
			videoTexture.loadData(videoPix);
		}
	}

	void draw(int w, int h, float a)
	{
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, a);
		videoTexture.draw(w, 0, -w, h);
		ofDisableAlphaBlending();
	}
};