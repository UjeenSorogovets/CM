#include "ofApp.h"
#include "UIdraw.h"
#include <string> 
#include <filesystem>

#include <functional>

int globalWidth = 1366;
int globalHeight = 768;
//int globalWidth = 1920;
//int globalHeight = 1080;
int widthDif = 1920 - globalWidth;
int heightDif = 1080 - globalHeight;

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

	cout << "[mousePressed] " << x << " " << y << " " << button << endl;

	isMouseClicked = true;

	int horizntalPanelEndX = horizontalPanel.startPosX + horizontalPanel.componentWidth * (horizontalPanel.components.size()) + horizontalPanel.offsetX * (horizontalPanel.components.size() - 1);
	int horizntalPanelEndY = horizontalPanel.startPosY + horizontalPanel.componentHeight;

	bool inHorizontalPanelCondition = (x >= horizontalPanel.startPosX && y >= horizontalPanel.startPosY && x <= horizntalPanelEndX && y <= horizntalPanelEndY);
	bool leftViewerCondition = inViewerCondition(x, y, &leftViewer);
	bool rightViewerCondition = inViewerCondition(x, y, &rightViewer);

	if (inHorizontalPanelCondition)
	{
		choosedButtonNumber = catchMediaButton(x, y, &horizontalPanel);
	}
	else if (leftViewerCondition)
	{
		playPausePlayer(&leftViewer,button);
	}
	else if (rightViewerCondition)
	{
		playPausePlayer(&rightViewer, button);
	}
	else
	{
		
	}
}

bool deleteInBorder(int x, int y, int xD,int yD, int w, int h)
{
	return (x >= xD && y >= yD && x <= xD + w && y <= yD + h);
}

bool deleteCondition(int x,int y, int xD,int yD, int w,int h)
{
	return deleteInBorder(x,y,xD,yD,w,h) ? true : false;
	
}

void ofApp::mouseReleased(int x, int y, int button) {


	isMouseClicked = false;
	if (choosedButtonNumber != -1)
	{
		helpImage.clear();

		for (int i = 0; i < horizontalPanel.components.size(); i++)
		{
			horizontalPanel.components[i].currentPosX = horizontalPanel.components[i].startPosX;
			horizontalPanel.components[i].currentPosY = horizontalPanel.components[i].startPosY;
		}

		isMousePressed = false;

		int res = choosedButtonNumber;

		x = mouseX;
		y = mouseY;


		bool topDeleteCondition    = deleteCondition(x, y, 0, 0, globalWidth, 20);
		bool leftDeleteCondition   = deleteCondition(x, y, 0, 0, 20, globalHeight);
		bool rightDeleteCondition  = deleteCondition(x, y, 0, globalHeight - 20, globalWidth, 20);
		bool bottomDeleteCondition = deleteCondition(x, y, globalWidth - 20, 0, 20, globalHeight);

		bool anyOfDeleteConditions = topDeleteCondition || leftDeleteCondition || rightDeleteCondition || bottomDeleteCondition;

		if (anyOfDeleteConditions)
		{
			cout << "DELETE" << endl;
			for (int i = 0; i < allComponents.size(); i++)
			{
				for (int j = 0; j < horizontalPanel.components.size(); j++)
				{
					if (allComponents[i].component == horizontalPanel.components[j].component)
					{
						//cout << "\n\nFOUND!" << endl;
						allComponents.erase(allComponents.begin() + i);
						i += 100;
						j += 100;
					}
				}
				
			}
			horizontalPanel.components.erase(horizontalPanel.components.begin() + res);
			
			auto newComponents = horizontalPanel.components;

			//newComponents = currentComponent.filterFunc(newComponents);

			int count = newComponents.size();
			cout << "newComponents size = " << count << endl;
			horizontalPanel.components.clear();
			for (int i = 0; i < count; i++)
			{
				horizontalPanel.push(newComponents[i]);
			}
			
		}
		else
		{
			bool leftViewerCondition = inViewerCondition(x, y, &leftViewer);
			bool rightViewerCondition = inViewerCondition(x, y, &rightViewer);

			if (leftViewerCondition)
			{
				runInCurrentPlayer(&leftViewer, res);
			}
			else if (rightViewerCondition)
			{
				runInCurrentPlayer(&rightViewer, res);
			}
			else
			{
				cout << "can't load!" << endl;
			}
		}
		

		choosedButtonNumber = -1;
	}
}



vector<Component> onlyImages(vector<Component>& components)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i].type != IMAGE)
		{
			components.erase(components.begin() + i);
			i--;
		}
	}

	return components;
}

vector<Component> onlyVideo(vector<Component>& components)
{
	cout << "only video!!!" << endl;
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i].type != VIDEO)
		{
			components.erase(components.begin() + i);
			i--;
		}
	}

	return components;
}

vector<Component> onlyFirstComponent(vector<Component>& components)
{
	for (int i = 1; i < components.size(); i++)
	{
		components.erase(components.begin() + i);
		i--;
	}

	return components;
}

void ofApp::setup()
{
	ofSetWindowPosition(0, 0);
	ofSetWindowShape(globalWidth, globalHeight);

	ofxDatGuiComponent* component;

	component = new ofxDatGuiButton("+");
	component->setPosition(x + 20, y + 20);
	component->setWidth(100, 0.0f);
	component->setHeight(100);
	component->onButtonEvent(this, &ofApp::addMediaClick);
	component->setOpacity(0.0f);
	components.push_back(component);

	component = new ofxDatGuiFRM();
	component->setPosition(0, 0);
	component->setHeight(20);
	components.push_back(component);

	/*component = new ofxDatGuiButton("+");
	component->setPosition(x + 20, 800);
	component->setWidth(100, 0.0f);
	component->setHeight(100);
	component->onButtonEvent(this, &ofApp::addFilterClick);
	component->setOpacity(0.0f);
	components.push_back(component);*/
	//
	horizontalPanel.startPosX = 250;
	horizontalPanel.startPosY = 30;
	horizontalPanel.offsetX = 20;
	horizontalPanel.offsetY = 0;
	//

	leftViewer = MediaViewer(250, 180, 800 - (widthDif / 2), 800 - (heightDif / 1.25));
	rightViewer = MediaViewer(1050 - (widthDif / 2), 180, 800 - (widthDif / 2), 800 - (heightDif / 1.25));

	verticalPanel.startPosX = 30;
	verticalPanel.startPosY = 150;
	verticalPanel.offsetX = 0;
	verticalPanel.offsetY = 10;
	verticalPanel.horizontal = false;

	verticalPanel.push(createFilterButton(this, "Default"));
	verticalPanel.push(createFilterButton(this, "Only Image", onlyImages));
	verticalPanel.push(createFilterButton(this, "Only Video", onlyVideo));
	verticalPanel.push(createFilterButton(this, "Only First", onlyFirstComponent));

	auto currentPath = fs::current_path().string() + "\\src\\videos\\";

	addMediaClickByPath(currentPath + "video3.mp4");
	addMediaClickByPath(currentPath + "video2.mp4");
	addMediaClickByPath(currentPath + "img1.png");
	addMediaClickByPath(currentPath + "img2.jpg");
	
	//rightViewer.imagePlayer.load("C:/Users/GAD/Desktop/image_2021-05-12_10-24-07.png");
	using namespace std::placeholders;

	webCam.initialize(std::bind(&ofApp::mousePressed, this, _1, _2, _3));
}

void updateAll(MediaViewer* mediaViewer)
{
	mediaViewer->imagePlayer.update();
	mediaViewer->videoPlayer.update();
}

void ofApp::update()
{
	for (int i = 0; i < components.size(); i++) components[i]->update();

	updateAll(horizontalPanel);
	updateAll(verticalPanel);

	updateAll(&leftViewer);
	updateAll(&rightViewer);

	helpImage.update();

	webCam.update();
}

void drawAll(InfoPanel panel)
{
	panel.myInput->draw();
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

void drawAll(FilterComponent component)
{
	component.component->draw();
	component.image.draw(component.currentPosX, component.currentPosY, component.width, component.height);
}

void drawAll(FiltersPanel componentPanel)
{
	for (int i = 0; i < componentPanel.components.size(); i++)
	{
		FilterComponent currentComponent = componentPanel.components[i];
		currentComponent.component->draw();
	}
}

void drawAll(MediaViewer mediaViewer)
{
	mediaViewer.infoPanel.myInput->draw();
	mediaViewer.infoPanel.myInput2->draw();
	mediaViewer.infoPanel.myInput3->draw();
	mediaViewer.infoPanel.myInput4->draw();


	//mediaViewer.infoPanel.myInput_->draw();
	//mediaViewer.infoPanel.myInput2_->draw();
	//mediaViewer.infoPanel.myInput3_->draw();
	//mediaViewer.infoPanel.myInput4_->draw();

	if (mediaViewer.isImageNow)
	{
		mediaViewer.imagePlayer.draw(mediaViewer.x, mediaViewer.y, mediaViewer.width, mediaViewer.height);
	}
	else
	{
		mediaViewer.videoPlayer.draw(mediaViewer.x, mediaViewer.y, mediaViewer.width, mediaViewer.height);
	}
}

void ofApp::draw()
{
	helpImage.draw(helpImageX, helpImageY, helpImageWidth, helpImageHeight);

	drawAll(horizontalPanel);
	drawAll(verticalPanel);

	drawAll(leftViewer);
	drawAll(rightViewer);


	for (int i = 0; i < components.size(); i++) components[i]->draw();

	ofNoFill(); // If we omit this and leave ofFill(), all the shapes will be filled!
	ofSetLineWidth(4.5); // A higher value will render thicker lines

	ofDrawRectangle(20, 20, 1880 - (widthDif), 1000 - (heightDif / 1.15));

	ofDrawRectangle(20, 20, 1880 - (widthDif), 120);

	ofDrawRectangle(20, 20, 200, 1000 - (heightDif / 1.15));


	if (isMousePressed)
	{
		ofDrawRectangle(leftViewer.startX, leftViewer.startY, leftViewer.maxWidth, leftViewer.maxHeight);

		ofDrawRectangle(rightViewer.startX, rightViewer.startY, rightViewer.maxWidth, rightViewer.maxHeight);

		// shorter notation is also possible
		ofColor red(255, 0, 0);
		ofSetColor(red);
		ofFill();

		ofDrawRectangle(0, 0, globalWidth, 20);
		ofDrawRectangle(0, 0, 20, globalHeight);
		ofDrawRectangle(globalWidth-20, 0, 20, globalHeight);
		ofDrawRectangle(0, globalHeight-20, globalWidth, 20);
	}


	webCam.draw(globalWidth, globalHeight, 150);
}


void ofApp::onFilterClick(ofxDatGuiButtonEvent e)
{
	int currentIndex = e.target->getIndex();
	FilterComponent currentComponent;
	cout << "onFilterClick index = " << currentIndex << endl;
	for (int i = 0; i < verticalPanel.components.size(); i++)
	{
		if (verticalPanel.components[i].index == currentIndex)
		{
			currentComponent = verticalPanel.components[i];
		}
	}

	auto newComponents = allComponents;

	newComponents = currentComponent.filterFunc(newComponents);

	int count = newComponents.size();
	cout << "newComponents size = " << count << endl;
	horizontalPanel.components.clear();
	for (int i = 0; i < count; i++)
	{
		horizontalPanel.push(newComponents[i]);
	}
	//
}

void ofApp::addMediaClickByPath(string path = "")
{
	auto mediaButton = createMediaButton(this, path);
	//
	allComponents.push_back(mediaButton);
	//
	horizontalPanel.push(mediaButton);
}

void ofApp::addMediaClick(ofxDatGuiButtonEvent e)
{
	//cout << "addMediaClick" << endl;
	auto mediaButton = createMediaButton(this);
	//
	allComponents.push_back(mediaButton);
	//
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

