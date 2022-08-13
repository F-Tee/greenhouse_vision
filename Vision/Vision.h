#pragma once
#include <opencv2/opencv.hpp>

class Vision {
private:
	const int fps = 20;

	cv::Mat imgRed;

public:
	static void onMouse(int event, int x, int y, int flags, void* param);

	void maskWindows(const cv::Mat& inputBGRimage);

	void trayDetection(std::string filename);
};