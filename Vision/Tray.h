#pragma once

#include "Tray.h"
#include "Cell.h"
#include <vector>
#include <opencv2/opencv.hpp>

class Tray {
private:
	int xCoordinate;
	int yCoordinate;
	std::vector<Cell> cells;
	cv::Point topLeftCorner;
	cv::Point bottomRightCorner;

public:
	Tray(int x1, int y1, int x2, int y2);
	void setCoordinates(std::vector<int> coordinates);
	void printCoordinates();
	std::vector<cv::Point> getCorners();
};

