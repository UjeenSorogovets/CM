#include "MediaViewer.h"
#include "Component.h"

InfoPanel::InfoPanel(int x_, int y_, int width_, int height_)
{
	startX = x_;
	startY = y_;
	x = x_;
	y = y_;
	maxHeight = height_;
	maxWidth = width_;
	height = height_;
	width = width_;

	auto halfWidth = width / 2;

	myInput = new ofxDatGuiTextInput("text input", "# open frameworks #");
	myInput->setWidth(width, 0);
	myInput->setPosition(x, y);


	myInput2 = new ofxDatGuiTextInput("text input 2", "# open frameworks #");
	myInput2->setWidth(width, 0);
	myInput2->setPosition(x, y + 20);


	myInput3 = new ofxDatGuiTextInput("text input 3", "# open frameworks #");
	myInput3->setWidth(width, 0);
	myInput3->setPosition(x, y + 40);


	myInput4 = new ofxDatGuiTextInput("text input 4", "# open frameworks #");
	myInput4->setWidth(width, 0);
	myInput4->setPosition(x, y + 60);

}

MediaViewer::MediaViewer(int x_, int y_, int width_, int height_, int infoHeight_ )
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

	int dif = 25;

	play = new ofxDatGuiButton("play");
	play->setPosition(x , y + height);
	play->setWidth(maxWidth/3, 0.0f);
	play->setHeight(25);

	pause = new ofxDatGuiButton("pause");
	pause->setPosition(x+ maxWidth/3, y + height);
	pause->setWidth(maxWidth / 3, 0.0f);
	pause->setHeight(25);

	stop = new ofxDatGuiButton("stop");
	stop->setPosition(x + (maxWidth / 3)+ (maxWidth / 3), y + height);
	stop->setWidth(maxWidth / 3, 0.0f);
	stop->setHeight(25);
	

	infoPanel = InfoPanel(x, y + height+dif, width, infoHeight);
}

void  MediaViewer::setInfoPanel(Component component)
{
	auto metaData = component.metaData;

	infoPanel.myInput->setText(metaData.contentPath.string());
	infoPanel.myInput2->setText("2");
	
}
