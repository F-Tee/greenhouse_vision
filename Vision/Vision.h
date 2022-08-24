#pragma once
#include <opencv2/opencv.hpp>
#include "Tray.h"

class Vision {
private:
	const int fps = 20;

	cv::Mat imgRed;

public:
	static std::vector<double> realPosition(int x, int y);

	bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2);

	void dotDetection(Tray tray, cv::Mat image);

	void maskContours(const cv::Mat& inputBGRimage);

	void trayDetection(std::string filename);
};