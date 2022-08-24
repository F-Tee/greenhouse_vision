#pragma once
#include <opencv2/opencv.hpp>

class Vision {
private:
	const int fps = 20;
	cv::Mat imgRed;
public:
	static std::vector<double> realPosition(int x, int y);
	static void onMouse(int event, int x, int y, int flags, void* param);
	void maskWindows(const cv::Mat& inputBGRimage);
	void trayDetection(std::string filename);
	void measureMask(std::string filename);
	void mouseMeasure(int event, int x, int y, int flags, void* param);
};