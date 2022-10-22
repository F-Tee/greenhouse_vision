#pragma once

static class Coordinates {
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
	int pixelTrayWidth;
public:

	std::vector<int> realPosition(int x, int y);

	std::vector<int> realCoordinates(int x, int y);

	void calculateDistance();

	void setPixelTrayWidth(int width);
};