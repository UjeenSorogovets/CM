#pragma once

#include "ofApp.h"
#include "AdditionalTypes.h"
#include <filesystem>

class MetaData
{
public:
	ofXml xmlRoot;
	ofVec2f size;
	ComponentType type;
	std::filesystem::path contentPath;
	std::filesystem::path xmlPath;
};
