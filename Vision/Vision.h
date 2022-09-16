#pragma once
#include <opencv2/opencv.hpp>
#include "Tray.h"

class Vision {
private:
	const int fps = 20;

	cv::Mat imgRed;

public:
	static void onMouse(int event, int x, int y, int flags, void* param);

	void drawContourCentres();

	void cellAverages();

	void calculateCellMeasurements(std::string filename, int tray, int dots);

	static std::vector<int> realPosition(int x, int y);

	void contourDetection();

	void initialiseTrays();

	void colourMask();

	void sortDots();

	void trayDetection(std::string filename);

	void initialiseCells(std::string filename, int tray, int dots);

	void drawCells();
};