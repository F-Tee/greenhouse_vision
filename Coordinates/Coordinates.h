#pragma once

class Coordinates {
private:
	const double fx = 276.558;
	const double fy = 1439.6;
	const double cx = 2.49999;
	const double cy = 3.99997;
	const double xDimensionTray = 18.2;
	const double yDimensionTray = 14.2;
	const double cameraHeight = 22.5;
	double trayDistance;
	double focalLength;
public:
	Coordinates();

	void calculateDistance(int pixelTrayWidth);

	double realCoordinates(int coordinate);
};