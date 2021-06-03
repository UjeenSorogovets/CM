#pragma once
#include "ofApp.h"

#include "MetaData.h"
#include "AdditionalTypes.h"
#include <filesystem>


class Component
{
public:

	std::filesystem::path baseXmlPath = std::filesystem::current_path() / "Xml";
	ofxDatGuiComponent* component;
	ofImage image;

	int index;

	int startPosX;
	int startPosY;

	int currentPosX;
	int currentPosY;

	int height;
	int width;

	ComponentType type;

	string path = "";

	MetaData metaData;

	bool fetchXml()
	{

		metaData.contentPath = path;
		metaData.xmlPath = baseXmlPath / (metaData.contentPath.filename().replace_extension("xml"));

		if (metaData.xmlRoot.load(metaData.xmlPath))
		{
			ofXml mainNode = metaData.xmlRoot.getChild("component");
			//mainNode.appendChild("contentPath").set(metaData.contentPath.c_str());
			string type = mainNode.getChild("type").getValue();
			if (type == "IMAGE")
			{
				metaData.type = IMAGE;
			}
			else if (type == "VIDEO")
			{
				metaData.type = VIDEO;
			}

			ofXml sizeNode = mainNode.getChild("size");
			metaData.size.x = sizeNode.getChild("w").getIntValue();
			metaData.size.y = sizeNode.getChild("h").getIntValue();
			return true;
		}
		else
		{
			metaData.type = type;
			metaData.size = ofVec2f(image.getWidth(), image.getHeight());
			////////
			ofXml mainNode = metaData.xmlRoot.appendChild("component");
			mainNode.appendChild("contentPath").set(metaData.contentPath);
			ofXml typeNode = mainNode.appendChild("type");
			switch (metaData.type)
			{
			case IMAGE:
				typeNode.set("IMAGE");
				break;
			case VIDEO:
				typeNode.set("VIDEO");
				break;
			default:
				typeNode.set("UNKNOWN");
				break;
			}
			ofXml sizeNode = mainNode.appendChild("size");
			sizeNode.appendChild("w").set((int)metaData.size.x);
			sizeNode.appendChild("h").set((int)metaData.size.y);
			std::filesystem::create_directories(metaData.xmlPath.parent_path());
			if (metaData.xmlRoot.save(metaData.xmlPath))
			{
				cout << "save xml to: [" << metaData.xmlPath << "]" << endl;
			}
			else
			{
				cout << "error save xml to: [" << metaData.xmlPath << "]" << endl;
			}
			return false;
		}
	}
};

class FilterComponent : public Component
{
public:
	vector<Component> components;
};