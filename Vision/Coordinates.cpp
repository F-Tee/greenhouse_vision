#include <iostream>
#include <vector>
#include <armadillo.hpp>
#include <opencv2/opencv.hpp>
#include "Coordinates.h"

std::pair<int, int> Coordinates::realCoordinates(int x, int y) {
	std::pair<int, int> coordinates;
	double aoc = x * fx / 1920;
	double co = cameraHeight * tan(aoc);
	double xCoord = 0 - co;
	coordinates.first = xCoord;
	double boc = y * fy / 1080;
	double bc = cameraHeight * tan(boc);
	double yCoord = 0 - bc;
	coordinates.second = yCoord;
	return coordinates;
}