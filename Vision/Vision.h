#pragma once
#include <opencv2/opencv.hpp>
#include "Tray.h"

class Vision {
private:
	const int fps = 20;

	cv::Mat imgRed;

public:
	static void onMouse(int event, int x, int y, int flags, void* param);

	void calculateCellMeasurements(std::vector<std::vector<cv::Point>> contours);

	static std::vector<int> realPosition(int x, int y);

	void dotDetection();

	void initialiseTrays(std::vector<std::vector<cv::Point>> contours, std::vector<cv::Vec4i> hierarchy);

	void colourMasks();

	void trayDetection(std::string filename);
};