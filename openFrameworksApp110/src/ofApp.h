#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "MediaViewer.h"
#include "Component.h"
#include "ComponentPanel.h"

namespace fs = std::filesystem;

static void resizePlayerToMedia(MediaViewer* mediaViewer, ComponentType mediaType)
{
	float height;
	float width;

	if (mediaType == VIDEO)
	{
		height = mediaViewer->videoPlayer.getHeight();
		width = mediaViewer->videoPlayer.getWidth();
	}
	else
	{
		height = mediaViewer->imagePlayer.getHeight();
		width = mediaViewer->imagePlayer.getWidth();
	}

	auto maxHeight = mediaViewer->maxHeight;
	auto maxWidth = mediaViewer->maxWidth;

	if (width > height)
	{
		cout << "width>height" << endl;
		mediaViewer->height = maxHeight / (width / height);
		mediaViewer->width = maxWidth;

		mediaViewer->y = mediaViewer->startY + (maxHeight - mediaViewer->height) / 2;
		mediaViewer->x = mediaViewer->startX;
	}
	else
	{
		cout << "width<height" << endl;
		mediaViewer->height = maxHeight;
		mediaViewer->width = maxWidth * (width / height);

		mediaViewer->y = mediaViewer->startY;
		mediaViewer->x = mediaViewer->startX + (maxWidth - mediaViewer->width) / 2;
	}

	cout << "height/width = " << height << "/" << width << endl;
}

static bool inViewerCondition(int x, int y, MediaViewer* mediaViewer)
{
	bool condition = (x >= mediaViewer->startX && y >= mediaViewer->startY) &&
		(x <= mediaViewer->startX + mediaViewer->maxWidth && y <= mediaViewer->startY + mediaViewer->maxHeight);
	return condition;
}

class ofApp : public ofBaseApp{

public:
	void mouseReleased(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void setup();
	void update();
	void draw();

	void addFilterClick(ofxDatGuiButtonEvent e);

	void addMediaClick(ofxDatGuiButtonEvent e);

	ofxDatGuiValuePlotter* plotter;
	vector<ofxDatGuiComponent*> components;

	void onButtonEvent(ofxDatGuiButtonEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void onDropdownEvent(ofxDatGuiDropdownEvent e);
	void onMatrixEvent(ofxDatGuiMatrixEvent e);
	void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
	void on2dPadEvent(ofxDatGui2dPadEvent e);
	void onTextInputEvent(ofxDatGuiTextInputEvent e);
		
};
