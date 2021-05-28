#include "ofApp.h"
#include <string> 

class MediaViewer
{
public:
	ofVideoPlayer videoPlayer;
	ofImage imagePlayer;

	int x;
	int y;

	int height;
	int width;

	bool isImageNow = true;

	MediaViewer()
	{

	}

	MediaViewer(int x_, int y_, int width_, int height_)
	{
		x = x_;
		y = y_;
		height = height_;
		width = width_;
	}
};

MediaViewer leftViewer;
MediaViewer rightViewer;

int x = 0;
int y = 0;
int p = 200;

bool isMousePressed = false;

enum ComponentType
{
	VIDEO,
	IMAGE,
	FILTER
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

	ofXml metaData;
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

ComponentPanel horizontalPanel;
ComponentPanel verticalPanel;

int catchMediaButton(int x, int y)
{
	int buttonNumber = -1;

	for (int i = 1; i < horizontalPanel.components.size(); i++)
	{
		Component currentComponent = horizontalPanel.components[i];

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
	if ((x >= horizontalPanel.startPosX && y >= horizontalPanel.startPosY) && (x <= horizontalPanel.startPosX + horizontalPanel.componentWidth && y <= horizontalPanel.startPosY + horizontalPanel.componentHeight))
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
		choosedButtonNumber = catchMediaButton(x, y);
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
		if (horizontalPanel.components[res].type == IMAGE)
		{
			cout << "IMAGE" << endl;
			mediaViewer->isImageNow = true;
			auto img = horizontalPanel.components[res].image;
			mediaViewer->imagePlayer = img;
		}
		else if (horizontalPanel.components[res].type == VIDEO)
		{
			cout << "VIDEO" << endl;

			mediaViewer->isImageNow = false;
			mediaViewer->videoPlayer.load(horizontalPanel.components[res].path);
			mediaViewer->videoPlayer.play();
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

void ofApp::mousePressed(int x, int y, int button) {
	//cout << "mousePressed" << endl;
	isMouseClicked = true;
	//cout << "isMouseClicked = " << isMouseClicked << endl;
	int horizntalPanelEndX = horizontalPanel.startPosX + horizontalPanel.componentWidth * (horizontalPanel.components.size())+horizontalPanel.offsetX * (horizontalPanel.components.size()-1);
	int horizntalPanelEndY = horizontalPanel.startPosY + horizontalPanel.componentHeight;
	cout <<"\nhorizntalPanelEndX = " << horizntalPanelEndX << endl;
	cout <<"horizntalPanelEndY = " << horizntalPanelEndY << endl;
	if (x>=horizontalPanel.startPosX && y>=horizontalPanel.startPosY &&x<= horizntalPanelEndX&&y<=horizntalPanelEndY)
	{
		cout << "maybe u in hor panel" << endl;
		choosedButtonNumber = catchMediaButton(x, y);
		cout <<"choosedButtonNumber = "<< choosedButtonNumber << endl;
	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	//cout << "mouseReleased" << endl;
	isMouseClicked = false;
	if (choosedButtonNumber!=-1)
	{
		cout << "isMouseClicked = " << isMouseClicked << endl;

		helpImage.clear();

		for (int i = 0; i < horizontalPanel.components.size(); i++)
		{
			horizontalPanel.components[i].currentPosX = horizontalPanel.components[i].startPosX;
			horizontalPanel.components[i].currentPosY = horizontalPanel.components[i].startPosY;
		}

		isMousePressed = false;

		//int res = stoi(e.target->getLabel());
		int res = choosedButtonNumber;

		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		cout << "\nonButtonEvent N : " << res << " at " << std::ctime(&end_time) << endl;

		int x = mouseX;
		int y = mouseY;
		cout << x << endl;
		cout << y << endl;

		if ((x >= leftViewer.x && y >= leftViewer.y) && (x <= leftViewer.x + leftViewer.width && y <= leftViewer.y + leftViewer.height))
		{
			cout << "leftMedia" << endl;
			runInCurrentPlayer(&leftViewer, res);
		}
		else if ((x >= rightViewer.x && y >= rightViewer.y) && (x <= rightViewer.x + rightViewer.width && y <= rightViewer.y + rightViewer.height))
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
	//cout << "isMouseClicked = " << isMouseClicked << endl;
	
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
		string xmlPath = path.substr(0, path.find_last_of(".")) + ".xml";
		ofVideoPlayer player;

		if (!component.metaData.load(xmlPath));
		{
			cout << "xml load error [" << xmlPath << "]" << endl;
		}
		auto rootXml = component.metaData.getChild("component");
		ofXml pathNode, typeNode;
		if (!rootXml)
		{
			rootXml = component.metaData.appendChild("component");
			typeNode = rootXml.appendChild("type");
			pathNode = rootXml.appendChild("path");
		}
		else
		{
			typeNode = rootXml.getChild("type");
			pathNode = rootXml.getChild("path");
		}
		player.load(path);
		if (player.getMoviePath() == "")
		{
			cout << "added image" << endl;
			image.load(path);
			component.type = IMAGE;
			typeNode.set("IMAGE");
		}
		else
		{
			cout << "added video" << endl;
			component.type = VIDEO;
			typeNode.set("VIDEO");
			player.play();
			player.setPaused(true);
			auto x = player.getPixels();
			image.setFromPixels(x);
		}
		component.path = path;
		pathNode.set(path);
		if (!component.metaData.save(xmlPath))
		{
			cout << "xml save error [" << xmlPath << "]" << endl;
		}
	}
	component.image = image;
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
		leftViewer.imagePlayer.draw(leftViewer.x, leftViewer.y, leftViewer.height, leftViewer.width);
	}
	else
	{
		leftViewer.videoPlayer.draw(leftViewer.x, leftViewer.y, leftViewer.height, leftViewer.width);
	}

	if (rightViewer.isImageNow)
	{
		rightViewer.imagePlayer.draw(rightViewer.x, rightViewer.y, rightViewer.height, rightViewer.width);
	}
	else
	{
		rightViewer.videoPlayer.draw(rightViewer.x, rightViewer.y, rightViewer.height, rightViewer.width);
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
		ofDrawRectangle(leftViewer.x, leftViewer.y, leftViewer.height, leftViewer.width);

		ofNoFill(); // If we omit this and leave ofFill(), all the shapes will be filled!
		ofSetLineWidth(4.5); // A higher value will render thicker lines
		ofDrawRectangle(rightViewer.x, rightViewer.y, rightViewer.height, rightViewer.width);
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

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	
		/*cout <<"isMouseClicked = " << isMouseClicked << endl;
	
		helpImage.clear();

		for (int i = 0; i < horizontalPanel.components.size(); i++)
		{
			horizontalPanel.components[i].currentPosX = horizontalPanel.components[i].startPosX;
			horizontalPanel.components[i].currentPosY = horizontalPanel.components[i].startPosY;
		}

		isMousePressed = false;
		choosedButtonNumber = -1;

		int res = stoi(e.target->getLabel());

		auto end = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		cout << "\nonButtonEvent N : " << res << " at " << std::ctime(&end_time) << endl;

		int x = mouseX;
		int y = mouseY;
		cout << x << endl;
		cout << y << endl;

		if ((x >= leftViewer.x && y >= leftViewer.y) && (x <= leftViewer.x + leftViewer.width && y <= leftViewer.y + leftViewer.height))
		{
			cout << "leftMedia" << endl;
			runInCurrentPlayer(&leftViewer, res);
		}
		else if ((x >= rightViewer.x && y >= rightViewer.y) && (x <= rightViewer.x + rightViewer.width && y <= rightViewer.y + rightViewer.height))
		{
			cout << "rightMedia" << endl;
			runInCurrentPlayer(&rightViewer, res);
		}
		else
		{
			cout << "can't load!" << endl;
		}
	*/
	
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