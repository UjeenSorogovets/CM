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

	myInput = new ofxDatGuiTextInput("Path", "####");
	myInput->setWidth(width, 0);
	myInput->setPosition(x, y);


	myInput2 = new ofxDatGuiTextInput("Color", "####");
	myInput2->setWidth(width, 0);
	myInput2->setPosition(x, y + 20);


	myInput3 = new ofxDatGuiTextInput("Luminescence", "####");
	myInput3->setWidth(width, 0);
	myInput3->setPosition(x, y + 40);


	myInput4 = new ofxDatGuiTextInput("Face count", "####");
	myInput4->setWidth(width, 0);
	myInput4->setPosition(x, y + 60);

	myInput5 = new ofxDatGuiTextInput("Rythm", "####");
	myInput5->setWidth(width, 0);
	myInput5->setPosition(x, y + 80);

	myInput6 = new ofxDatGuiTextInput("edge histogramm", "####");
	myInput6->setWidth(width, 0);
	myInput6->setPosition(x, y + 100);

	myInput7 = new ofxDatGuiTextInput("texture descriptor", "####\n####\n####");
	myInput7->setWidth(width,0);
	myInput7->setPosition(x, y + 120);
	myInput7->setHeight(60);

	//myPlotter = new ofxDatGuiValuePlotter("edge histogramm", 0, 255);
	//myPlotter->setWidth(width, 0);
	//myPlotter->setPosition(x, y + 120);

}

MediaViewer::MediaViewer(int x_, int y_, int width_, int height_, int infoHeight_)
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
	play->setPosition(x, y + height);
	play->setWidth(maxWidth / 3, 0.0f);
	play->setHeight(25);

	pause = new ofxDatGuiButton("pause");
	pause->setPosition(x + maxWidth / 3, y + height);
	pause->setWidth(maxWidth / 3, 0.0f);
	pause->setHeight(25);

	stop = new ofxDatGuiButton("stop");
	stop->setPosition(x + (maxWidth / 3) + (maxWidth / 3), y + height);
	stop->setWidth(maxWidth / 3, 0.0f);
	stop->setHeight(25);


	infoPanel = InfoPanel(x, y + height + dif, width, infoHeight);
}

void  MediaViewer::setInfoPanel(Component component)
{
	auto metaData = component.metaData;


	infoPanel.myInput->setText(metaData.contentPath.string());
	infoPanel.myInput2->setText(
		"[" + to_string(metaData.meanColor[0] * 255) +
		", " + to_string(metaData.meanColor[1] * 255) +
		", " + to_string(metaData.meanColor[1] * 255) + "]"
	);

	infoPanel.myInput2->setBackgroundColor(metaData.meanColor);
	infoPanel.myInput2->setLabelColor(metaData.meanColor.getInverted());

	infoPanel.myInput3->setText(to_string(metaData.meanLuminacance));
	infoPanel.myInput4->setText(to_string(metaData.faceCount));
	infoPanel.myInput5->setText(to_string(metaData.videoRythm));
	infoPanel.myInput6->setText(
		to_string(metaData.edgeHistogramm[5])+ " * [" +
		to_string(metaData.edgeHistogramm[0]) + ", " +
		to_string(metaData.edgeHistogramm[1]) + ", " +
		to_string(metaData.edgeHistogramm[2]) + ", " +
		to_string(metaData.edgeHistogramm[3]) + ", " +
		to_string(metaData.edgeHistogramm[4]) + "]"
	);
	string textDesc = "";
	for (size_t i = 0; i < metaData.textureDesc.size(); i++)
	{
		for (size_t j = 0; j < metaData.textureDesc[i].size(); j++)
		{
			textDesc += to_string(metaData.textureDesc[i][j]) + ", ";
		}
		textDesc += "\n";
	}
	infoPanel.myInput7->setText(textDesc);
	/*infoPanel.myPlotter->setValue(55);
	infoPanel.myPlotter->setValue(5);
	infoPanel.myPlotter->setValue(105);*/

}
