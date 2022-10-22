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
	// Camera calibration matrix
	arma::mat camMtx = { fx, 0, cx, 0, fy, cy, 0, 0, 1 };
	camMtx.reshape(3, 3);
	// Inverse camera calibration matrix
	arma::mat invCamMtx = camMtx.i();
	// Create an array from x, y and 1
	arma::mat uv1 = { (double)x, (double)y, 1 };
	// Transpose matrix
	uv1 = uv1.t();
	// Multiply by rotation matrix
	// Calculate dot product of inverse camera matrix and pixel coordinates
	double xyz = arma::dot(invCamMtx, uv1);
	// Subtract translation matrix
	xyz -
	// Multiply by the inverse rotation matrix
	// Return xyz
}
*/

/*
void Coordinates::calculateTranslation() {
	double u = cx * 100;
	double v = cy * 100;
	// First the centre XYZ and then real world xyz values
	std::vector<cv::Point3d> worldPoints;
	// Coordinates of those points as pixel values
	std::vector<cv::Point2d> imagePoints;
	// Camera calibration matrix

	// Distortion coefficient
	double distCoeffs[8];
	// rvec and tvec
	cv::Mat rvec, tvec;
	// Solvepnp
	cv::solvePnP(worldPoints, imagePoints, camMtx, distCoeffs, rvec, tvec);

}
*/

void Coordinates::calculatePositionAngle(int x = 960, int y = 540) {
	// Gets real world x value from centre of camera's origin
	double aoc = x * fx / 1920;
	// Gets real world x value from centre of camera's origin
	double boc = y * fy / 1080;
	std::cout << "x position: " << aoc << " y position: " << boc;
}