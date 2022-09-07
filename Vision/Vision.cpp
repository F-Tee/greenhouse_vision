#include "Vision.h"
#include "Cell.h"
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

Mat image;
Mat imageHSV;

Mat trayImage;
Tray tray;

std::vector<Cell> cellList;

void Vision::onMouse(int event, int x, int y, int flags, void* param) {
	if (event != EVENT_LBUTTONDOWN)
	{
		return;
	}

	std::cout << std::endl;
	std::cout << "Camera position:" << std::endl;
	std::cout << "x: " << x << std::endl;
	std::cout << "y: " << y << std::endl;

	std::vector<Cell>* cellVec = (std::vector<Cell>*)param;
	Cell newCell(x, y);
	cellVec->push_back(newCell);
	return;
}

void Vision::calculateCellMeasurements(std::vector<std::vector<Point>> contours) {
	std::vector<std::vector<int>> cellCoordinates;
	std::vector<int> xCoords;
	std::vector<int> yCoords;
	std::vector<Point> points;
	double cellWidth;
	double cellHeight;


	namedWindow("Contours", WINDOW_NORMAL);
	imshow("Contours", image);
	resizeWindow("Contours", 1600, 800);
	setMouseCallback("Contours", onMouse, &cellList);
	waitKey(0);

	std::vector<int> xList;
	std::vector<int> yList;
	int xAvg;
	int yAvg;
	Cell lastCell = cellList[0];

	for (int i = 1; i < cellList.size(); i++) {
		if (cellList[i].getCellCoordinateX() > lastCell.getCellCoordinateX()) {
			xList.push_back(cellList[i].getCellCoordinateX());
		}
		else {
			yList.push_back(cellList[i].getCellCoordinateY());
		}
		lastCell = cellList[i];
	}

	int xSum = 0;
	for (int i = 0; i < xList.size(); i++) {
		xSum += xList[i];
	}
	xAvg = xSum / xList.size();
	std::cout << "X average: " << xAvg << std::endl;

	int ySum = 0;
	for (int i = 0; i < yList.size(); i++) {
		ySum += yList[i];
	}
	yAvg = ySum / yList.size();
	std::cout << "Y average: " << yAvg << std::endl;
}

std::vector<int> Vision::realPosition(int x, int y) {
	double xFov = 2 * (atan(xDimensionTray / (2 * fx)));
	double yFov = 2 * (atan(yDimensionTray / (2 * fy)));
	double x2 = x / cameraHeight;
	double y2 = y / cameraHeight;
	double u;
	double v;

	u = fx + x2 + cx;
	v = fy + y2 + cy;

	std::vector<int> coordinates;
	coordinates.push_back(u);
	coordinates.push_back(v);
	return coordinates;
}

void Vision::dotDetection() {
	// Canny edge detection
	Mat cannyOutput;
	Canny(trayImage, cannyOutput, 50, 150, 3);
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

	// Initialise trays
	initialiseTrays(contours, hierarchy);
}

void Vision::initialiseTrays(std::vector<std::vector<Point>> contours, std::vector<Vec4i> hierarchy) {
	Mat drawing = image.clone();
	Point start;
	Point end;
	std::vector<Point> points;
	// Draw largest contour and centre circle
	if (contours.size() < 1) {
		std::cout << "First dot not detected" << std::endl;
		return;
	}
	// Maximum and minimum x and y coordinates
	for (int i = contours.size() - 1; i != contours.size() - 20; i--) {
		drawContours(drawing, contours, (i), Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point());
		Moments m = moments(contours[i], true);
		Point p(m.m10 / m.m00, m.m01 / m.m00);
		points.push_back(p);
		circle(drawing, p, 5, Scalar(0, 0, 255), -1);
		std::cout << "x: " << p.x << std::endl;
		std::cout << "y: " << p.y << std::endl;
		std::cout << std::endl;
	}

	Rect rectangle = cv::boundingRect(points);
	cv::rectangle(drawing, rectangle.tl(), rectangle.br(), Scalar(0, 140, 255), 10, 8);

	// Create tray object and add to trays vector
	Moments m = moments(contours[contours.size() - 1], true);
	Point p(m.m10 / m.m00, m.m01 / m.m00);
	std::vector<int> coordinates = realPosition(p.x, p.y);
	tray.setCoordinates(coordinates);
	tray.printCoordinates();
	// Show the image
	namedWindow("Contours", WINDOW_NORMAL);
	imshow("Contours", drawing);
	resizeWindow("Contours", 1600, 800);
	waitKey(0);
	calculateCellMeasurements(contours);
}

void Vision::colourMasks() {
	assert(!image.empty());

	// Converts image from BGR to HSV
	cvtColor(image, imageHSV, COLOR_BGR2HSV);

	// Detects yellow colour
	inRange(imageHSV, Scalar(20, 100, 100), Scalar(30, 255, 255), trayImage);
	dotDetection();
}

void Vision::trayDetection(std::string filename) {
	image = imread(filename, IMREAD_COLOR);

	colourMasks();

}

int main()
{
	Vision vision;
	vision.trayDetection("multiple_dots.jpg");
}
