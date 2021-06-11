#pragma once
#include "ofMain.h"
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

	bool fetchXml();
	private:

		ofColor getColor(cv::Mat &img);
		int getLum(cv::Mat &img);
		void getFrames(cv::Mat *frame, double &rythm, const string path, const ComponentType type);
		void getType();
		int countFaces(cv::Mat &frame);
		vector<double> getEdgeHist(cv::Mat &frame);
		vector< vector<double>> getTexture(cv::Mat &frame);
};

vector<Component> withoutFilter(vector<Component>& components);

class FilterComponent : public Component
{
public:
	vector<Component> components;
	//std::function<const int&()> F([] { return 42; });
	/*std::function<void(vector<Component>)> filterFunc;*/
	std::function<vector<Component>(vector<Component>&)> filterFunc = withoutFilter;
};