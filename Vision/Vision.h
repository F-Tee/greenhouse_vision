#pragma once
#include <opencv2/opencv.hpp>
#include "Tray.h"

class Vision {
private:
	const int fps = 20;

	cv::Mat imgRed;

public:
	static std::vector<double> realPosition(int x, int y);

	// void dotDetection(Tray tray, cv::Mat image, cv::Mat& input);

	void dotDetection(Tray tray, cv::Mat image);

	void maskWindows(const cv::Mat& inputBGRimage);

	void maskWindowsContour(const cv::Mat& inputBGRimage);

	void trayDetection(std::string filename);
};