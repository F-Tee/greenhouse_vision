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
const double cameraHeight = 22.5;

static Mat imgRed;

std::vector<Tray> trays;

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

void Vision::dotDetection(Tray tray, cv::Mat image) {
	Mat cannyOutput;
	// Canny edge detection
	Canny(image, cannyOutput, 50, 150, 3);
	// Gaussian blur
	Mat blur;
	GaussianBlur(cannyOutput, blur, Size(3, 3), 0);
	// Find contours
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	findContours(blur, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// Sort contours
	sort(contours.begin(), contours.end(), [](const std::vector<Point>& c1, const std::vector<Point>& c2) {
		return contourArea(c1, false) < contourArea(c2, false);
		});
	Mat drawing(blur.size(), CV_8UC3, Scalar(255, 255, 255));
	Scalar colour = Scalar(167, 151, 0);
	Scalar colourCentre = Scalar(0, 0, 255);

	drawContours(drawing, contours, (contours.size() - 1), colour, 2, 8, hierarchy, 0, Point());
	Moments m = moments(contours[contours.size() - 1], true);
	Point p(m.m10 / m.m00, m.m01 / m.m00);
	circle(drawing, p, 5, colourCentre, -1);
	// Create tray object and add to trays vector
	std::vector<double> coordinates = realPosition(p.x, p.y);
	Tray trayObj;
	trayObj.setCoordinates(coordinates);
	trayObj.printCoordinates();
	trays.push_back(trayObj);
	// Show the image
	namedWindow("Contours", WINDOW_NORMAL);
	imshow("Contours", drawing);
	resizeWindow("Contours", 1600, 800);
	waitKey(0);
}

void Vision::maskContours(const Mat& inputBGRimage) {
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
	dotDetection(trayRed, imgRed);

	// Detects yellow colour
	inRange(imageHSV, Scalar(20, 100, 100), Scalar(30, 255, 255), imgYellow);
	dotDetection(trayYellow, imgYellow);


	// Detects blue colour
	inRange(imageHSV, Scalar(100, 130, 130), Scalar(105, 255, 255), imgBlue);
	dotDetection(trayBlue, imgBlue);

}

void Vision::trayDetection(std::string filename) {
	Mat image = imread(filename, IMREAD_COLOR);

	// maskWindows(image);
	maskContours(image);
}

int main()
{
	Vision vision;
	vision.trayDetection("tray_dots.jpg");
}
