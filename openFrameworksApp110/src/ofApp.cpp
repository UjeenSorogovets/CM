#include "ofApp.h"
#include "UIdraw.h"
#include <string> 

void ofApp::mouseDragged(int x, int y, int button) {
	if (choosedButtonNumber == -1)
	{
		choosedButtonNumber = catchMediaButton(x, y, &horizontalPanel);
		if (choosedButtonNumber != -1 && choosedButtonNumber <= horizontalPanel.components.size())
		{
			offsetDragX = horizontalPanel.components[choosedButtonNumber].startPosX - x;
			offsetDragY = horizontalPanel.components[choosedButtonNumber].startPosY - y;
		}
	}
	else if (choosedButtonNumber != -1 && choosedButtonNumber <= horizontalPanel.components.size())
	{
		isMousePressed = true;

		helpImage = horizontalPanel.components[choosedButtonNumber].image;
		helpImageX = horizontalPanel.components[choosedButtonNumber].startPosX;
		helpImageY = horizontalPanel.components[choosedButtonNumber].startPosY;
		helpImageWidth = horizontalPanel.components[choosedButtonNumber].width;
		helpImageHeight = horizontalPanel.components[choosedButtonNumber].height;

		horizontalPanel.components[choosedButtonNumber].currentPosX = x + offsetDragX;
		horizontalPanel.components[choosedButtonNumber].currentPosY = y + offsetDragY;
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	//cout << "mousePressed" << endl;
	isMouseClicked = true;
	//cout << "isMouseClicked = " << isMouseClicked << endl;
	int horizntalPanelEndX = horizontalPanel.startPosX + horizontalPanel.componentWidth * (horizontalPanel.components.size()) + horizontalPanel.offsetX * (horizontalPanel.components.size() - 1);
	int horizntalPanelEndY = horizontalPanel.startPosY + horizontalPanel.componentHeight;
	//cout << "\nhorizntalPanelEndX = " << horizntalPanelEndX << endl;
	//cout << "horizntalPanelEndY = " << horizntalPanelEndY << endl;

	bool inHorizontalPanelCondition = (x >= horizontalPanel.startPosX && y >= horizontalPanel.startPosY && x <= horizntalPanelEndX && y <= horizntalPanelEndY);
	bool leftViewerCondition = inViewerCondition(x, y, &leftViewer);
	bool rightViewerCondition = inViewerCondition(x, y, &rightViewer);

	if (inHorizontalPanelCondition)
	{
		choosedButtonNumber = catchMediaButton(x, y, &horizontalPanel);
		//cout << "choosedButtonNumber = " << choosedButtonNumber << endl;
	}
	else if (leftViewerCondition)
	{
		//cout << "leftViewerCondition click!" << endl;
		playPausePlayer(&leftViewer);
	}
	else if (rightViewerCondition)
	{
		//cout << "rightViewerCondition click!" << endl;
		playPausePlayer(&rightViewer);
	}
	else
	{
		//cout << "mousePressed for nothing" << endl;
	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	isMouseClicked = false;
	if (choosedButtonNumber != -1)
	{
		//cout << "isMouseClicked = " << isMouseClicked << endl;

		helpImage.clear();

		for (int i = 0; i < horizontalPanel.components.size(); i++)
		{
			horizontalPanel.components[i].currentPosX = horizontalPanel.components[i].startPosX;
			horizontalPanel.components[i].currentPosY = horizontalPanel.components[i].startPosY;
		}

		isMousePressed = false;

		int res = choosedButtonNumber;

		int x = mouseX;
		int y = mouseY;
		//cout << x << endl;
		//cout << y << endl;

		bool leftViewerCondition = inViewerCondition(x,y, &leftViewer);
		bool rightViewerCondition = inViewerCondition(x, y, &rightViewer);

		if (leftViewerCondition)
		{
			//cout << "leftMedia" << endl;
			runInCurrentPlayer(&leftViewer, res);
		}
		else if (rightViewerCondition)
		{
			//cout << "rightMedia" << endl;
			runInCurrentPlayer(&rightViewer, res);
		}
		else
		{
			cout << "can't load!" << endl;
		}

		choosedButtonNumber = -1;
	}
}

void createAddMediaButton(int x,int y,int width,int height,ofApp* app, vector<ofxDatGuiComponent*> components)
{
	ofxDatGuiComponent* component;

	component = new ofxDatGuiButton("+");
	component->setPosition(x + 20, y + 20);
	component->setWidth(width, 0.0f);
	component->setHeight(height);
	component->onButtonEvent(app, &ofApp::addMediaClick);
	component->setOpacity(0.0f);
	components.push_back(component);
}

//void createAddFilterButton(int x, int y, int width, int height, ofApp* app, vector<ofxDatGuiComponent*> components)
//{
//	ofxDatGuiComponent* component;
//
//	component = new ofxDatGuiButton("+");
//	component->setPosition(x + 20, y + 20);
//	component->setWidth(width, 0.0f);
//	component->setHeight(height);
//	component->onButtonEvent(app, &ofApp::addMediaClick);
//	component->setOpacity(0.0f);
//	components.push_back(component);
//}

//int globalWidth = 1280;
//int globalHeight = 1024;
int globalWidth = 1366;
int globalHeight = 768;
//int globalWidth = 1920;
//int globalHeight = 1080;
void ofApp::setup()
{
	ofSetWindowPosition(0, 0);
	/*ofSetWindowShape(1920, 1080);*/
	ofSetWindowShape(globalWidth, globalHeight);

	ofxDatGuiComponent* component;
	
	component = new ofxDatGuiButton("+");
	component->setPosition(x + 20, y + 20);
	component->setWidth(100, 0.0f);
	component->setHeight(100);
	component->onButtonEvent(this, &ofApp::addMediaClick);
	component->setOpacity(0.0f);
	components.push_back(component);

	component = new ofxDatGuiButton("+");
	component->setPosition(x + 20, 800);
	component->setWidth(100, 0.0f);
	component->setHeight(100);
	component->onButtonEvent(this, &ofApp::addFilterClick);
	component->setOpacity(0.0f);
	components.push_back(component);
	//
	horizontalPanel.startPosX = 250;
	horizontalPanel.startPosY = 30;
	horizontalPanel.offsetX = 20;
	horizontalPanel.offsetY = 0;
	//
	int widthDif = 1920 - globalWidth;
	int heightDif = 1080 - globalHeight;
	leftViewer = MediaViewer(250, 180, 800-(widthDif/2), 800- (heightDif/1.25));
	rightViewer = MediaViewer(1050- (widthDif / 2), 180, 800- (widthDif / 2), 800 - (heightDif/1.25));

	verticalPanel.startPosX = 60;
	verticalPanel.startPosY = 150;
	verticalPanel.offsetX = 0;
	verticalPanel.offsetY = 20;
	verticalPanel.horizontal = false;

	for (int i = 0; i < 4; i++)
	{
		auto x = createFilterButton(this);
		verticalPanel.push(x);
	}
	//rightViewer.imagePlayer.load("C:/Users/GAD/Desktop/image_2021-05-12_10-24-07.png");

}

void ofApp::update()
{
	for (int i = 0; i < components.size(); i++) components[i]->update();

	updateAll(horizontalPanel);
	updateAll(verticalPanel);

	leftViewer.imagePlayer.update();
	leftViewer.videoPlayer.update();

	rightViewer.imagePlayer.update();
	rightViewer.videoPlayer.update();
	
	helpImage.update();
}

void drawAll(Component component)
{
	component.component->draw();
	component.image.draw(component.currentPosX, component.currentPosY, component.width, component.height);
}

void drawAll(ComponentPanel componentPanel)
{
	for (int i = 0; i < componentPanel.components.size(); i++)
	{
		Component currentComponent = componentPanel.components[i];
		drawAll(currentComponent);
	}
}

void ofApp::draw()
{
	helpImage.draw(helpImageX, helpImageY, helpImageWidth, helpImageHeight);
	
	drawAll(horizontalPanel);
	drawAll(verticalPanel);

	if (leftViewer.isImageNow)
	{
		leftViewer.imagePlayer.draw(leftViewer.x, leftViewer.y, leftViewer.width, leftViewer.height);
	}
	else
	{
		leftViewer.videoPlayer.draw(leftViewer.x, leftViewer.y, leftViewer.width, leftViewer.height);
	}

	if (rightViewer.isImageNow)
	{
		rightViewer.imagePlayer.draw(rightViewer.x, rightViewer.y, rightViewer.width, rightViewer.height);
	}
	else
	{
		rightViewer.videoPlayer.draw(rightViewer.x, rightViewer.y, rightViewer.width, rightViewer.height);
	}

	for (int i = 0; i < components.size(); i++) components[i]->draw();

	ofNoFill(); // If we omit this and leave ofFill(), all the shapes will be filled!
	ofSetLineWidth(4.5); // A higher value will render thicker lines
	ofDrawRectangle(20, 20, 1880, 1000);

	ofNoFill(); // If we omit this and leave ofFill(), all the shapes will be filled!
	ofSetLineWidth(4.5); // A higher value will render thicker lines
	ofDrawRectangle(20, 20, 1880, 120);

	ofNoFill(); // If we omit this and leave ofFill(), all the shapes will be filled!
	ofSetLineWidth(4.5); // A higher value will render thicker lines
	ofDrawRectangle(20, 20, 200, 1000);

	if (isMousePressed)
	{
		ofNoFill(); // If we omit this and leave ofFill(), all the shapes will be filled!
		ofSetLineWidth(4.5); // A higher value will render thicker lines
		ofDrawRectangle(leftViewer.startX, leftViewer.startY, leftViewer.maxWidth, leftViewer.maxHeight);

		ofNoFill(); // If we omit this and leave ofFill(), all the shapes will be filled!
		ofSetLineWidth(4.5); // A higher value will render thicker lines
		ofDrawRectangle(rightViewer.startX, rightViewer.startY, rightViewer.maxWidth, rightViewer.maxHeight);
	}
}

void ofApp::onFilterClick(ofxDatGuiButtonEvent e)
{
	cout << "onFilterClick" <<e.target->getIndex() <<endl;
}

void ofApp::addFilterClick(ofxDatGuiButtonEvent e)
{
	cout << "addFilterClick" << endl;
	auto x = createFilterButton(this);

	verticalPanel.push(x);
}

void ofApp::addMediaClick(ofxDatGuiButtonEvent e)
{
	cout << "addMediaClick" << endl;
	auto mediaButton = createMediaButton(this);
	horizontalPanel.push(mediaButton);
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
	cout << "onToggleEvent: " << e.target->getLabel() << "::" << e.target->getChecked() << endl;
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
	cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
	cout << "onDropdownEvent: " << e.child << endl;
}

void ofApp::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
	cout << "onMatrixEvent: " << e.child << "::" << e.enabled << endl;
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
	cout << "onColorPickerEvent: " << e.color << endl;
}

void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e)
{
	cout << "on2dPadEvent: " << e.x << "::" << e.y << endl;
}

void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
	cout << "onButtonEvent: " << e.text << endl;
}

