#pragma once

class MediaViewer
{
public:
	ofVideoPlayer videoPlayer;
	ofImage imagePlayer;

	int startX;
	int startY;

	int x;
	int y;

	int maxHeight;
	int maxWidth;

	int height;
	int width;

	bool isImageNow = true;

	MediaViewer()
	{

	}

	MediaViewer(int x_, int y_, int width_, int height_)
	{
		startX = x_;
		startY = y_;
		x = x_;
		y = y_;
		maxHeight = height_;
		maxWidth = width_;
		height = height_;
		width = width_;
	}
};
