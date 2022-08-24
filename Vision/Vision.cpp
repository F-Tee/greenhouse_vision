#include "Vision.h"
#include "Tray.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/aruco.hpp"


#include <iostream>
#include <sstream>
#include <fstream>
#include <stdint.h>

using namespace cv;


const int fps = 20;

const double fx = 276.558;
const double fy = 1439.6;
const double cx = 2.49999;
const double cy = 3.99997;
const double xDimensionTray = 18.2;
const double yDimensionTray = 14.2;
const double cameraHeight = 41.5;

static Mat imgRed;

std::vector<double> Vision::realPosition(int x, int y) {
	double xFov = 2 * (atan(xDimensionTray / (2 * fx)));
	double yFov = 2 * (atan(yDimensionTray / (2 * fy)));
	double x2 = x / cameraHeight;
	double y2 = y / cameraHeight;
	double u;
	double v;

	u = fx + x2 + cx;
	v = fy + y2 + cy;

	std::vector<double> coordinates;
	coordinates.push_back(u);
	coordinates.push_back(v);
	return coordinates;
}

void Vision::onMouse(int event, int x, int y, int flags, void* param) {
	if (event != EVENT_LBUTTONDOWN)
	{
		return;
	}

	std::cout << std::endl;
	std::cout << "Camera position:" << std::endl;
	std::cout << "x: " << x << std::endl;
	std::cout << "y: " << y << std::endl;

	Tray* pThis = (Tray*)param;
	std::vector<double> coordinates = realPosition(x, y);
	pThis->setCoordinates(coordinates);
	pThis->printCoordinates();
	return;
}

void Vision::measureMask(std::string filename) {
	Mat imageHSV;
	Mat mask;
	Mat image = imread(filename, IMREAD_COLOR);
	Mat input = image.clone();
	cvtColor(input, imageHSV, COLOR_BGR2HSV);
	inRange(imageHSV, Scalar(50, 100, 100), Scalar(70, 255, 255), mask);
}


void Vision::maskWindows(const Mat& inputBGRimage) {
	Mat input = inputBGRimage.clone();
	Mat imageHSV;
	Mat imgRed1, imgRed2;
	Mat imgYellow;
	Mat imgBlue;
	Tray trayRed;
	Tray trayBlue;
	Tray trayYellow;
	assert(!input.empty());

	// Converts image from BGR to HSV
	cvtColor(input, imageHSV, COLOR_BGR2HSV);

	// Detects red colour
	inRange(imageHSV, Scalar(0, 70, 50), Scalar(10, 255, 255), imgRed1);
	inRange(imageHSV, Scalar(170, 70, 50), Scalar(180, 255, 255), imgRed2);
	imgRed = imgRed1 | imgRed2;
	namedWindow("Red Tray Detection", WINDOW_NORMAL);
	std::cout << "Red Tray Detection" << std::endl;
	while (trayRed.getInitialised() != true) {
		imshow("Red Tray Detection", imgRed);
		resizeWindow("Red Tray Detection", 1600, 800);
		setMouseCallback("Red Tray Detection", onMouse, &trayRed);
		waitKey(1);
	}
	destroyWindow("Red Tray Detection");

	// Detects yellow colour
	inRange(imageHSV, Scalar(20, 100, 100), Scalar(30, 255, 255), imgYellow);
	namedWindow("Yellow Tray Detection", WINDOW_NORMAL);
	std::cout << "Yellow Tray Detection" << std::endl;
	while (trayYellow.getInitialised() != true) {
		imshow("Yellow Tray Detection", imgYellow);
		resizeWindow("Yellow Tray Detection", 1600, 800);
		setMouseCallback("Yellow Tray Detection", onMouse, &trayYellow);
		waitKey(1);
	}
	destroyWindow("Yellow Tray Detection");

	// Detects blue colour
	inRange(imageHSV, Scalar(100, 130, 130), Scalar(105, 255, 255), imgBlue);
	std::cout << "Blue Tray Detection" << std::endl;
	namedWindow("Blue Tray Detection", WINDOW_NORMAL);
	while (trayBlue.getInitialised() != true) {
		imshow("Blue Tray Detection", imgBlue);
		resizeWindow("Blue Tray Detection", 1600, 800);
		setMouseCallback("Blue Tray Detection", onMouse, &trayBlue);
		waitKey(1);
	}
	destroyWindow("Blue Tray Detection");
}

void Vision::trayDetection(std::string filename) {
	Mat image = imread(filename, IMREAD_COLOR);

	maskWindows(image);

}

int main()
{
	Vision vision;
	// vision.measureMask("single_tray.jpg");
	vision.trayDetection("tray_dots.jpg");
}