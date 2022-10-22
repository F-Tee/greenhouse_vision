#include <iostream>
#include <vector>
#include "Coordinates.h"
#include <armadillo.hpp>

void Coordinates::calculateDistance() {
	focalLength = (pixelTrayWidth * cameraHeight) / xDimensionTray;
	std::cout << "Focal length: " << focalLength << std::endl;
	// D’ = (W x F) / P
	trayDistance = (xDimensionTray * focalLength) / pixelTrayWidth;
	std::cout << "Tray distance from camera: " << trayDistance << " cm" << std::endl;
}

std::vector<int> Coordinates::realPosition(int x, int y) {
	double xFov = 2 * (atan(xDimensionTray / (2 * fx)));
	double yFov = 2 * (atan(yDimensionTray / (2 * fy)));
	double x2 = x / trayDistance;
	double y2 = y / trayDistance;
	double u;
	double v;

	u = fx + x2 + cx;
	v = fy + y2 + cy;

	std::vector<int> coordinates;
	coordinates.push_back(u);
	coordinates.push_back(v);
	return coordinates;
}

/*
std::vector<int> Coordinates::realCoordinates(int x, int y) {
	// Create an array from x, y and 1
	arma::mat uv_1 = { (double)x, (double)y, 1 };
	// Transpose matrix
	uv_1 = uv_1.t();
	// Multiply by rotation matrix
	// Calculate dot product of inverse camera matrix and pixel coordinates
	// Subtract translation matrix
	// Multiply by the inverse rotation matrix
	// Return xyz
}
*/

void Coordinates::setPixelTrayWidth(int width) {
	pixelTrayWidth = width;
}