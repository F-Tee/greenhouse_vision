#pragma once
#include <opencv2/opencv.hpp>
#include "Tray.h"

class Vision {
private:
	const int fps = 20;

	cv::Mat imgRed;

public:
	static void onMouse(int event, int x, int y, int flags, void* param);

	void drawCellBoxes();

	void drawContourCentres();

	void cellAverages();

	void calculateCellMeasurements(std::string filename);

	static std::vector<int> realPosition(int x, int y);

	void contourDetection();

	void initialiseTrays();

	void colourMask();

	void trayDetection(std::string filename);
};