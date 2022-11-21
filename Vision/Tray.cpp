#include "Tray.h"
#include "Cell.h"
#include <vector>
#include <iostream>
#include "Cell.h"

std::vector<Cell> cells;

cv::Point topLeftCorner;
cv::Point bottomRightCorner;

Tray::Tray(int x1, int y1, int x2, int y2) {
	topLeftCorner = cv::Point(x1, y1);
	bottomRightCorner = cv::Point(x2, y2);
}

void Tray::setCoordinates(std::vector<int> coordinates) {
	xCoordinate = coordinates.at(0);
	yCoordinate = coordinates.at(1);
}

void Tray::printCoordinates() {
	std::cout << "Real position:" << std::endl;
	std::cout << "x: " << xCoordinate << std::endl;
	std::cout << "y: " << yCoordinate << std::endl;
	std::cout << std::endl;
}

std::vector<cv::Point> Tray::getCorners() {
	std::vector<cv::Point> corners = { topLeftCorner, bottomRightCorner };
	return corners;
}