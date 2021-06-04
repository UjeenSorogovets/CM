#pragma once

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

		InfoPanel()
		{

		}

		InfoPanel(int x_, int y_, int width_, int height_)
		{
			startX = x_;
			startY = y_;
			x = x_;
			y = y_;
			maxHeight = height_;
			maxWidth = width_;
			height = height_;
			width = width_;

			
			//myInput->setText("Freddy");
			myInput = new ofxDatGuiTextInput("text input", "# open frameworks #");
			myInput->setPosition(x, y);

			myInput2 = new ofxDatGuiTextInput("text input 2", "# open frameworks #");
			myInput2->setPosition(x, y+20);

			myInput3 = new ofxDatGuiTextInput("text input 3", "# open frameworks #");
			myInput3->setPosition(x, y + 40);

			myInput4 = new ofxDatGuiTextInput("text input 4", "# open frameworks #");
			myInput4->setPosition(x, y+ 60);
			
		}
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

	MediaViewer(int x_, int y_, int width_, int height_, int infoHeight_ = 200)
	{
		infoHeight = infoHeight_;

		startX = x_;
		startY = y_;
		x = x_;
		y = y_;
		maxHeight = height_ - infoHeight;
		maxWidth = width_;
		height = height_ - infoHeight;
		width = width_;

		//infoPanel = InfoPanel();
		infoPanel =  InfoPanel(x,y+height,width, infoHeight_);
	}
};
