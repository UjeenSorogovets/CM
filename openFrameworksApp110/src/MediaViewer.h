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

	//vector<ofxDatGuiComponent> components;
	ofxDatGuiTextInput* myInput;
	ofxDatGuiTextInput* myInput2;
	ofxDatGuiTextInput* myInput3;
	ofxDatGuiTextInput* myInput4;

	//ofxDatGuiTextInput* myInput_;
	//ofxDatGuiTextInput* myInput2_;
	//ofxDatGuiTextInput* myInput3_;
	//ofxDatGuiTextInput* myInput4_;


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

	MediaViewer()
	{
	}

	MediaViewer(int x_, int y_, int width_, int height_, int infoHeight_ = 200);

	void setInfoPanel(Component component);
};
