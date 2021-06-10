#include "UIdraw.h"


vector<Component> allComponents;
ComponentPanel horizontalPanel;
//static ComponentPanel verticalPanel;
FiltersPanel verticalPanel;


void resizePlayerToMedia(MediaViewer* mediaViewer, ComponentType mediaType)
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
		//cout << "width>height" << endl;
		mediaViewer->height = maxHeight / (width / height);
		mediaViewer->width = maxWidth;

		mediaViewer->y = mediaViewer->startY + (maxHeight - mediaViewer->height) / 2;
		mediaViewer->x = mediaViewer->startX;
	}
	else
	{
		//cout << "width<height" << endl;
		mediaViewer->height = maxHeight;
		mediaViewer->width = maxWidth * (width / height);

		mediaViewer->y = mediaViewer->startY;
		mediaViewer->x = mediaViewer->startX + (maxWidth - mediaViewer->width) / 2;
	}

	//cout << "height/width = " << height << "/" << width << endl;
}

bool inViewerCondition(int x, int y, MediaViewer* mediaViewer)
{
	bool condition = (x >= mediaViewer->startX && y >= mediaViewer->startY) &&
		(x <= mediaViewer->startX + mediaViewer->maxWidth && y <= mediaViewer->startY + mediaViewer->maxHeight);
	return condition;
}



FilterComponent createFilterButton(ofApp* ofApp, string label, std::function<vector<Component>(vector<Component>)> filterFunc)
{
	FilterComponent component;

	component.component = new ofxDatGuiButton(label);
	component.path = label;
	component.type = FILTER;
	component.component->onButtonEvent(ofApp, &ofApp::onFilterClick);
	component.width = 150;
	component.height = 50;
	component.index = component.component->getIndex();
	component.filterFunc = filterFunc;

	return component;
	//std::function<void(vector<Component>)> filterFunc
}

FilterComponent createFilterButton(ofApp* ofApp, string label)
{
	FilterComponent component;

	component.component = new ofxDatGuiButton(label);
	component.path = label;
	component.type = FILTER;
	component.component->onButtonEvent(ofApp, &ofApp::onFilterClick);
	component.width = 150;
	component.height = 50;
	component.index = component.component->getIndex();

	return component;
	//
}

Component createMediaButton(ofApp* ofApp, string defaultPath)
{
	Component component;
	component.component = new ofxDatGuiButton("---");
	component.component->setOpacity(0.0f);
	//component.component->onButtonEvent(ofApp, &ofApp::onButtonEvent);

	ofImage image;

	if (defaultPath != "")
	{
		string path = defaultPath;
		ofVideoPlayer player;

		//cout << "play load:" << player.load(path) << endl;
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

		image.clear();
		player.close();

		if (!component.fetchXml())
		{
			cout << "xml load error [" << component.metaData.xmlPath << "]" << endl;
		}
	}
	else
	{
		ofFileDialogResult result = ofSystemLoadDialog("Load file");
		if (result.bSuccess)
		{
			string path = result.getPath();
			ofVideoPlayer player;

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
	}
	return component;
}

void runInCurrentPlayer(MediaViewer* mediaViewer, int res)
{
	try {
		auto mediaType = horizontalPanel.components[res].type;
		if (mediaType == IMAGE)
		{
			mediaViewer->videoPlayer.stop();

			mediaViewer->isImageNow = true;
			auto img = horizontalPanel.components[res].image;
			mediaViewer->imagePlayer = img;

			resizePlayerToMedia(mediaViewer, mediaType);
		}
		else if (mediaType == VIDEO)
		{
			mediaViewer->isImageNow = false;
			mediaViewer->videoPlayer.load(horizontalPanel.components[res].path);
			mediaViewer->videoPlayer.play();

			resizePlayerToMedia(mediaViewer, mediaType);
		}
		else
		{
			cout << "FILTER OR OTHER" << endl;
		}
		cout << "setInfoPanel" << endl;

		mediaViewer->setInfoPanel(horizontalPanel.components[res]);
	}
	catch (std::invalid_argument e) {
		cout << "Caught Invalid Argument Exception\n";
	}
}

void playPausePlayer(MediaViewer* mediaViewer, int button)
{
	if (!mediaViewer->isImageNow)
	{
		bool isPlaying = mediaViewer->videoPlayer.isPlaying();
		if (button == 2)
		{
			mediaViewer->videoPlayer.stop();
			mediaViewer->videoPlayer.play();
			mediaViewer->videoPlayer.setPaused(true);
		}
		else if (isPlaying)
		{
			mediaViewer->videoPlayer.setPaused(true);
		}
		else
		{
			mediaViewer->videoPlayer.play();
		}
	}
}

int catchMediaButton(int x, int y, ComponentPanel* componentPanel)
{
	int buttonNumber = -1;

	for (int i = 1; i < componentPanel->components.size(); i++)
	{
		Component currentComponent = componentPanel->components[i];

		if ((x >= currentComponent.currentPosX && y >= currentComponent.currentPosY) && (x <= currentComponent.currentPosX + currentComponent.width && y <= currentComponent.currentPosY + currentComponent.height))
		{
			//cout << currentComponent.component->getLabel() << endl;

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

void updateAll(FilterComponent component)
{
	component.component->update();
}

void updateAll(FiltersPanel componentPanel)
{
	for (int i = 0; i < componentPanel.components.size(); i++)
	{
		FilterComponent currentComponent = componentPanel.components[i];
		updateAll(currentComponent);
	}
}