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
	ofFloatColor meanColor;
	double meanLuminacance;
	double videoRythm = 0;
	int faceCount = 0;
	vector<double> edgeHistogramm;
	vector<vector<double>> textureDesc;
	std::filesystem::path contentPath;
	std::filesystem::path xmlPath;

	//typedef tuple <string, string> labelTextPair;

	//vector<tuple <string, string>> getTuples()
	//{
	//	vector<tuple <string, string>> tuples;

	//	tuples.push_back(make_tuple("Name", "Test name"));
	//	tuples.push_back(make_tuple("Size", "100500"));
	//	tuples.push_back(make_tuple("Info", "Nothing"));

	//	return tuples;
	//}
};
