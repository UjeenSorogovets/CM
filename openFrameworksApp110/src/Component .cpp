#include "Component.h"

using namespace std;
ofFloatColor Component::getColor(cv::Mat *img)
{
	cv::Scalar s = cv::mean(*img);
	cout << s << endl;
	return ofFloatColor(s.val[0], s.val[1], s.val[2], s.val[3]);
}

double Component::getLum(cv::Mat &img)
{
	cv::Mat gray;
	cv::cvtColor(img, gray, CV_BGR2GRAY);
	return (getColor(&gray))[0];
}

void Component::getFrames(cv::Mat *_frame, double &rythm,  const string path, const ComponentType type)
{

	if (type == IMAGE)
	{
		*_frame= cv::Mat(ofxCv::toCv(ofImage(path)));
	}
	if (type ==  VIDEO)
	{
		cv::VideoCapture cap(path);
		double fps = cap.get(cv::CAP_PROP_FPS);
		int framesN = cap.get(cv::CAP_PROP_FRAME_COUNT);
		cv::Mat prevHist[3];
		double prevHistMean = 0;
		if (!cap.isOpened())
		{
			cout << "Video read error" << endl;
			return;
		}
		cout << "[Video] " << fps << " "<< framesN<<endl;
		int k = 0;
		for (size_t i = 0; i < framesN; i++)
		{
			cv::Mat frame;
			cap.read(frame);
			if (frame.empty())
			{
				cout << "empty frame "<<i << endl;
				break;
			}

			//cout << "[hist mean] " << cv::mean(frame) << endl;
			//ofxCv::drawMat(frame,0,0);
			//cv::Mat hist; 
			const int channels = 0;
			const int histSize = 255;
			const float range[] = { 0, 256 };
			const float* histRange = { range };

			vector<cv::Mat> bgr_planes;
			cv::split(frame, bgr_planes);
			cv::Mat hist[3];
			cv::calcHist( &bgr_planes[0], 1, 0, cv::Mat(), hist[0], 1, &histSize, &histRange, true, false);
			cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), hist[1], 1, &histSize, &histRange, true, false);
			cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), hist[2], 1, &histSize, &histRange, true, false);
			//cout << "[hist mean]" << cv::mean(hist[0]) << endl;
			if (!prevHist[0].empty())
			{
				
				cv::Mat diff[3];
				cv::absdiff(hist[0], prevHist[0], diff[0]);
				cv::absdiff(hist[1], prevHist[1], diff[1]);
				cv::absdiff(hist[2], prevHist[2], diff[2]);
				double histMean = (cv::mean(diff[0]).val[0] + cv::mean(diff[1]).val[0] + cv::mean(diff[2]).val[0]) / 3;
				if (prevHistMean / histMean < 0.1)
				{
					*_frame = frame;
					k++;
				}
				prevHistMean = histMean;
			}
			prevHist[0] = hist[0];
			prevHist[1] = hist[1];
			prevHist[2] = hist[2];
		}
		cap.release();
		//frames->push_back(cv::Mat(ofxCv::toCv(player.getPixels())));

		rythm =k/(framesN/fps) ;

	}

}

void Component::getType()
{
	ofVideoPlayer player;

	if (image.load(path))
	{
		cout << "added image" << endl;
		this->type = IMAGE;
	}
	else
	{
		if (player.load(path)) {
			cout << "added video" << endl;
			this->type = VIDEO;
			player.play();
			player.setPaused(true);
			auto x = player.getPixels();
			image.setFromPixels(x);
		}
		else
		{
			cout << "error open file" << endl;
			this->type = UNKNOWN;
		}
	}
	this->image = image;
}

bool Component::fetchXml()
	{
	//cout << "[fetchXml]" << endl;
		metaData.contentPath = path;
		metaData.xmlPath = baseXmlPath / (metaData.contentPath.filename().replace_extension("xml"));
		getType(); 
		metaData.type = type;
		cv::Mat frame;
		double rythm;
		getFrames(&frame, metaData.videoRythm, metaData.contentPath.string(), metaData.type);
		cout <<frame.size() << endl;

		cout << "[rythm]" << metaData.videoRythm << endl;
		metaData.meanColor = getColor(&frame);
		cout << "[lcolor]" << metaData.meanColor << endl;
		//metaData.meanLuminacance = getLum(frame);
		//cout << "[lum]" << metaData.meanLuminacance << endl;
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

vector<Component> withoutFilter(vector<Component>& components)
{
	std::cout << "withoutFilter"<< endl;

	return components;
}