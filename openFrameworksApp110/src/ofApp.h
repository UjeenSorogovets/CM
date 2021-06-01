#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

namespace fs = std::filesystem;

enum ComponentType
{
	UNKNOWN = 0,
	VIDEO,
	IMAGE,
	FILTER
};

class MetaData
{
public:
	ofXml xmlRoot;
	ofVec2f size;
	ComponentType type;
	fs::path contentPath;
	fs::path xmlPath;
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

	MediaViewer()
	{

	}

	MediaViewer(int x_, int y_, int width_, int height_)
	{
		startX = x_;
		startY = y_;
		x = x_;
		y = y_;
		maxHeight = height_;
		maxWidth = width_;
		height = height_;
		width = width_;
	}
};

class Component
{
public:
	ofxDatGuiComponent* component;
	ofImage image;

	int startPosX;
	int startPosY;

	int currentPosX;
	int currentPosY;

	int height;
	int width;

	ComponentType type;

	string path = "";

	MetaData metaData;

	bool fetchXml();
};

class ComponentPanel
{
public:
	vector<Component> components;

	int startPosX;
	int startPosY;

	int componentHeight = 100;
	int componentWidth = 100;

	int offsetX;
	int offsetY;

	bool horizontal = true;

	void push(Component component)
	{
		if (component.path != "")
		{
			int size = components.size();
			component.startPosX = startPosX;
			component.startPosY = startPosY;

			component.component->setHeight(componentHeight);
			component.component->setWidth(componentWidth, 0);

			component.height = componentHeight;
			component.width = componentWidth;

			if (size == 0)
			{
				component.component->setPosition(startPosX, startPosY);

				cout << "add" << component.startPosX << endl;
			}
			else
			{
				if (horizontal)
				{
					component.startPosX = startPosX + component.width * (size)+offsetX * (size);
					component.component->setLabel(to_string(size));
					cout << "add" << component.startPosX << endl;
				}
				else
				{
					component.startPosY = startPosY * (size + 1) + offsetY * (size);
				}
				component.component->setPosition(component.startPosX, component.startPosY);
			}

			component.currentPosX = component.startPosX;
			component.currentPosY = component.startPosY;

			/*cout << "start:" << endl;
			cout << "X = " << component.startPosX << "\nY = " << component.startPosY << endl;
			cout << "current:" << endl;
			cout << "X = " << component.currentPosX << "\nY = " << component.currentPosY << endl;*/

			components.push_back(component);
		}
	}
};



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
