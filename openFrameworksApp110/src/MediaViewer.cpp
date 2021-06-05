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


	/*myInput_ = new ofxDatGuiTextInput("text input", "# open frameworks #");
	myInput_->setWidth(halfWidth, 0);
	myInput_->setPosition(x + halfWidth, y);

	myInput2_ = new ofxDatGuiTextInput("text input", "# open frameworks #");
	myInput2_->setWidth(halfWidth, 0);
	myInput2_->setPosition(x + halfWidth, y + 20);

	myInput3_ = new ofxDatGuiTextInput("text input", "# open frameworks #");
	myInput3_->setWidth(halfWidth, 0);
	myInput3_->setPosition(x + halfWidth, y + 40);

	myInput4_ = new ofxDatGuiTextInput("text input", "# open frameworks #");
	myInput4_->setWidth(halfWidth, 0);
	myInput4_->setPosition(x + halfWidth, y+ 60);*/
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

	infoPanel = InfoPanel(x, y + height, width, infoHeight);
}

void  MediaViewer::setInfoPanel(Component component)
{
	auto metaData = component.metaData;

	infoPanel.myInput->setText(metaData.contentPath.string());
	infoPanel.myInput2->setText("2");
	
}
