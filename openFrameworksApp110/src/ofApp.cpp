#include "ofApp.h"
#include <string> 

namespace fs = std::filesystem;
//fs::path baseXmlPath = fs::current_path() / "Xml";
MediaViewer leftViewer;
MediaViewer rightViewer;

int x = 0;
int y = 0;
int p = 200;

bool isMousePressed = false;

ComponentPanel horizontalPanel;
ComponentPanel verticalPanel;

int catchMediaButton(int x, int y, ComponentPanel* componentPanel)
{
	int buttonNumber = -1;

	for (int i = 1; i < componentPanel->components.size(); i++)
	{
		Component currentComponent = componentPanel->components[i];

		if ((x >= currentComponent.currentPosX && y >= currentComponent.currentPosY) && (x <= currentComponent.currentPosX + currentComponent.width && y <= currentComponent.currentPosY + currentComponent.height))
		{
			cout << currentComponent.component->getLabel() << endl;

			try {
				int res = stoi(currentComponent.component->getLabel());
				return res;
			}
			catch (std::invalid_argument e) {
				//cout << "Caught Invalid Argument Exception from catchMediaButton \n";
			}
		}
	}
	if ((x >= componentPanel->startPosX && y >= componentPanel->startPosY) && (x <= componentPanel->startPosX + componentPanel->componentWidth && y <= componentPanel->startPosY + componentPanel->componentHeight))
	{
		return 0;
	}
	return buttonNumber;
}

ofImage helpImage;
int helpImageX;
int helpImageY;
int helpImageWidth;
int helpImageHeight;

int choosedButtonNumber = -1;
int offsetDragX = 0;
int offsetDragY = 0;

bool isMouseClicked = false;

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

void runInCurrentPlayer(MediaViewer* mediaViewer, int res)
{
	try {
		auto mediaType = horizontalPanel.components[res].type;
		if (mediaType == IMAGE)
		{
			cout << "IMAGE" << endl;
			//stop video if get image
			mediaViewer->videoPlayer.stop();
			
			mediaViewer->isImageNow = true;
			auto img = horizontalPanel.components[res].image;
			mediaViewer->imagePlayer = img;
			
			resizePlayerToMedia(mediaViewer, mediaType);
		}
		else if (mediaType == VIDEO)
		{
			cout << "VIDEO" << endl;

			mediaViewer->isImageNow = false;
			mediaViewer->videoPlayer.load(horizontalPanel.components[res].path);
			mediaViewer->videoPlayer.play();

			resizePlayerToMedia(mediaViewer, mediaType);
		}
		else
		{
			cout << "FILTER OR OTHER" << endl;
		}
	}
	catch (std::invalid_argument e) {
		cout << "Caught Invalid Argument Exception\n";
	}
}

void playPausePlayer(MediaViewer* mediaViewer)
{
	if (!mediaViewer->isImageNow)
	{
		bool isPlaying = mediaViewer->videoPlayer.isPlaying();
		cout << isPlaying << endl;
		if (isPlaying)
		{
			mediaViewer->videoPlayer.setPaused(true);
		}
		else
		{
			mediaViewer->videoPlayer.play();
			
		}
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
		cout << "choosedButtonNumber = " << choosedButtonNumber << endl;
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
		cout << "mousePressed for nothing" << endl;
	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	isMouseClicked = false;
	if (choosedButtonNumber != -1)
	{
		cout << "isMouseClicked = " << isMouseClicked << endl;

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
		cout << x << endl;
		cout << y << endl;

		bool leftViewerCondition = inViewerCondition(x,y, &leftViewer);
		bool rightViewerCondition = inViewerCondition(x, y, &rightViewer);

		if (leftViewerCondition)
		{
			cout << "leftMedia" << endl;
			runInCurrentPlayer(&leftViewer, res);
		}
		else if (rightViewerCondition)
		{
			cout << "rightMedia" << endl;
			runInCurrentPlayer(&rightViewer, res);
		}
		else
		{
			cout << "can't load!" << endl;
		}

		choosedButtonNumber = -1;
	}
}

Component createFilterButton(ofApp* ofApp)
{
	Component component;
	auto testLabel = "tag " + to_string(verticalPanel.components.size());
	component.component = new ofxDatGuiButton(testLabel);
	//component.component->setOpacity(0.0f);
	component.path = testLabel;
	component.type = FILTER;
	//component.component->onButtonEvent(ofApp, &ofApp::onButtonEvent);

	return component;
}

Component createMediaButton(ofApp* ofApp)
{
	Component component;
	component.component = new ofxDatGuiButton("---");
	component.component->setOpacity(0.0f);
	component.component->onButtonEvent(ofApp, &ofApp::onButtonEvent);

	ofImage image;

	ofFileDialogResult result = ofSystemLoadDialog("Load file");
	if (result.bSuccess)
	{
		string path = result.getPath();
		ofVideoPlayer player;

		cout << "play load:" << player.load(path) << endl;
		if (image.load(path))
		{
			cout << "added image" << endl;
			component.type = IMAGE;
		}
		else
		{
			if (player.load(path)) {
				cout << "added video" << endl;
				component.type = VIDEO;
				player.play();
				player.setPaused(true);
				auto x = player.getPixels();
				image.setFromPixels(x);
			}
			else
			{
				cout << "error open file" << endl;
				component.type = UNKNOWN;
			}
		}
		component.image = image;
		component.path = path;
		if (!component.fetchXml())
		{
			cout << "xml load error [" << component.metaData.xmlPath << "]" << endl;
		}
	}
	return component;
}

void ofApp::setup()
{
	ofSetWindowPosition(0, 0);
	ofSetWindowShape(1920, 1080);

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

	leftViewer = MediaViewer(250, 180, 800, 800);
	rightViewer = MediaViewer(1050, 180, 800, 800);

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

void updateAll(Component component)
{
	component.component->update();
}

void updateAll(ComponentPanel componentPanel)
{
	for (int i = 0; i < componentPanel.components.size(); i++)
	{
		Component currentComponent = componentPanel.components[i];
		updateAll(currentComponent);
	}
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
	//
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
	//
	helpImage.draw(helpImageX, helpImageY, helpImageWidth, helpImageHeight);
	//
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

