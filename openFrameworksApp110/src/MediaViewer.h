#pragma once
#include "ofxDatGui.h"
class Component;
class InfoPanel
{
public:
	int startX;
	int startY;

	int x;
	int y;

	int maxHeight;
	int maxWidth;

	int height;
	int width;

	ofxDatGuiTextInput* myInput;
	ofxDatGuiTextInput* myInput2;
	ofxDatGuiTextInput* myInput3;
	ofxDatGuiTextInput* myInput4;
	ofxDatGuiTextInput* myInput5;
	ofxDatGuiTextInput* myInput6;
	//ofxDatGuiValuePlotter* myPlotter;

	InfoPanel()
	{

	}

	InfoPanel(int x_, int y_, int width_, int height_);
};

class MediaViewer
{
public:
	ofVideoPlayer videoPlayer;
	ofImage imagePlayer;

	int startX;
	int startY;

	int x;
	int y;

	int maxHeight;
	int maxWidth;

	int height;
	int width;

	bool isImageNow = true;

	int infoHeight;

	InfoPanel infoPanel;

	ofxDatGuiButton* play;
	ofxDatGuiButton* pause;
	ofxDatGuiButton* stop;

	MediaViewer()
	{
	}

	MediaViewer(int x_, int y_, int width_, int height_, int infoHeight_ = 200);

	void setInfoPanel(Component component);
};
