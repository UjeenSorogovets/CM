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

	string videoPath = "";
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
		int size = components.size();
		component.component->setLabel(to_string(size));

		component.startPosX = startPosX;
		component.startPosY = startPosY;
		//
		component.currentPosX = component.startPosX;
		component.currentPosY = component.startPosY;
		//
		component.component->setHeight(componentHeight);
		component.component->setWidth(componentWidth, 0);

		component.height = componentHeight;
		component.width = componentWidth;
		if (component.videoPath != "")
		{
			if (size == 0)
			{
				//cout << "if push!" << endl;
				component.component->setPosition(startPosX, startPosY);
				components.push_back(component);
			}
			else
			{
				if (horizontal)
				{
					component.startPosX = startPosX * (size + 1) + offsetX * (size);
				}
				else
				{
					component.startPosY = startPosY * (size + 1) + offsetY * (size);
				}

				//cout << "else push!" << endl;
				component.component->setPosition(component.startPosX, component.startPosY);
				components.push_back(component);
			}

			int x = component.component->getX();
			int y = component.component->getY();
			//cout << "X = " << x << "\nY = " << y << endl;
			//cout << endl;
			//cout << "X = " << component.startPosX << "\nY = " << component.startPosY << endl;
		}
	}
};

ComponentPanel horizontalPanel;

void ofApp::mouseDragged(int x, int y, int button) {
	isMousePressed = true;

	if (horizontalPanel.components.size() > 0)
	{
		horizontalPanel.components[0].currentPosX = x;
		horizontalPanel.components[0].currentPosY = y;

		horizontalPanel.components[0].component->setPosition(x, y);
	}
}
//

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
		player.load(path);
		if (player.getMoviePath() == "")
		{
			image.load(path);
			component.videoPath = "image";
		}
		else
		{
			component.videoPath = path;
			player.play();
			player.setPaused(true);
			auto x = player.getPixels();
			image.setFromPixels(x);
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

	horizontalPanel.startPosX = 250;
	horizontalPanel.startPosY = 30;
	horizontalPanel.offsetX = 20;
	horizontalPanel.offsetY = 0;

	leftViewer = MediaViewer(250, 180, 800, 800);
	rightViewer = MediaViewer(1050, 180, 800, 800);
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

	leftViewer.imagePlayer.update();
	leftViewer.videoPlayer.update();

	rightViewer.imagePlayer.update();
	rightViewer.videoPlayer.update();
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
	drawAll(horizontalPanel);

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

void ofApp::addMediaClick(ofxDatGuiButtonEvent e)
{
	cout << "+" << endl;
	auto x = createMediaButton(this);
	horizontalPanel.push(x);
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	for (int i = 0; i < horizontalPanel.components.size(); i++)
	{
		horizontalPanel.components[i].currentPosX = horizontalPanel.components[i].startPosX;
		horizontalPanel.components[i].currentPosY = horizontalPanel.components[i].startPosY;
	}
	for each (Component component in horizontalPanel.components)
	{
		component.currentPosX = component.startPosX;
		component.currentPosY = component.startPosY;
	}
	//
	isMousePressed = false;

	cout << "onButtonEvent: " << e.target->getLabel() << endl;

	int x = mouseX;
	int y = mouseY;
	//cout << x << endl;
	//cout << y << endl;
	cout << "" << endl;

	if ((x >= leftViewer.x && y >= leftViewer.y) && (x <= leftViewer.x + leftViewer.width && y <= leftViewer.y + leftViewer.height))
	{
		try {
			int res = stoi(e.target->getLabel());
			cout << "Integer: " << res << endl;
			if (horizontalPanel.components[res].videoPath == "image")
			{
				cout << "IMAGE" << endl;
				leftViewer.isImageNow = true;
				auto img = horizontalPanel.components[res].image;
				leftViewer.imagePlayer = img;
			}
			else
			{
				leftViewer.isImageNow = false;
			}
			leftViewer.videoPlayer.load(horizontalPanel.components[res].videoPath);
		}
		catch (std::invalid_argument e) {
			cout << "Caught Invalid Argument Exception\n";
		}
		leftViewer.videoPlayer.play();
	}
	else if ((x >= rightViewer.x && y >= rightViewer.y) && (x <= rightViewer.x + rightViewer.width && y <= rightViewer.y + rightViewer.height))
	{
		cout << "rightMedia" << endl;
		try {
			int res = stoi(e.target->getLabel());
			cout << "Integer: " << res << endl;
			if (horizontalPanel.components[res].videoPath == "image")
			{
				cout << "IMAGE" << endl;
				rightViewer.isImageNow = true;
				auto img = horizontalPanel.components[res].image;
				rightViewer.imagePlayer = img;
			}
			else
			{
				rightViewer.isImageNow = false;
			}
			rightViewer.videoPlayer.load(horizontalPanel.components[res].videoPath);
		}
		catch (std::invalid_argument e) {
			cout << "Caught Invalid Argument Exception\n";
		}
		rightViewer.videoPlayer.play();
	}
	else
	{
		cout << "can't load!" << endl;
	}
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