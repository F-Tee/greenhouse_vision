#include <iostream>
#include <vector>
#include <armadillo.hpp>
#include <opencv2/opencv.hpp>
#include "Coordinates.h"

Coordinates::Coordinates() {}

void Coordinates::calculateDistance(int pixelTrayWidth) {
	focalLength = (pixelTrayWidth * cameraHeight) / xDimensionTray;
	std::cout << "Focal length: " << focalLength << std::endl;
	// D’ = (W x F) / P
	trayDistance = (xDimensionTray * focalLength) / pixelTrayWidth;
	std::cout << "Tray distance from camera: " << trayDistance << " cm" << std::endl;
}

double Coordinates::realCoordinates(int coordinate) {
	std::vector<double> coordinates;
	double aoc = coordinate * fx / 1920;
	double co = cameraHeight * tan(aoc);
	double coord = 0 - co;
	return coord;
}