#pragma once

#include "ofMain.h"
//#include "Component.h"
//#include "ComponentPanel.h"
#include "ofxDatGui.h"
#include "MediaViewer.h"
#include "WebCamGrabber.h"
namespace fs = std::filesystem;

class ofApp : public ofBaseApp{

public:

	MediaViewer leftViewer;
	MediaViewer rightViewer;

	int x = 0;
	int y = 0;
	int p = 200;

	bool isMousePressed = false;

	WebCamGrabber webCam;

	ofImage helpImage;
	int helpImageX;
	int helpImageY;
	int helpImageWidth;
	int helpImageHeight;

	int choosedButtonNumber = -1;
	int offsetDragX = 0;
	int offsetDragY = 0;

	bool isMouseClicked = false;

	void mouseReleased(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void setup();
	void update();
	void draw();

	void onFilterClick(ofxDatGuiButtonEvent e);

	//void addFilterClick(ofxDatGuiButtonEvent e);

	void addMediaClick(ofxDatGuiButtonEvent e);

	ofxDatGuiValuePlotter* plotter;
	vector<ofxDatGuiComponent*> components;

	void addMediaClickByPath(string path);

	void onButtonEvent(ofxDatGuiButtonEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void onDropdownEvent(ofxDatGuiDropdownEvent e);
	void onMatrixEvent(ofxDatGuiMatrixEvent e);
	void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
	void on2dPadEvent(ofxDatGui2dPadEvent e);
	void onTextInputEvent(ofxDatGuiTextInputEvent e);
		
};
