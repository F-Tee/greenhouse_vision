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

const int cellDistanceX = 574;
const int cellDistanceY = 574;

int traySize;
int dotNumber;

Mat image;
Mat imageHSV;
Mat drawing;

Mat trayImage;
Tray tray;

std::vector<std::vector<Point>> contours;
std::vector<Vec4i> hierarchy;

std::vector<Cell> cellList;
int xAvg;
int yAvg;

std::vector<Point> dotCentres;
std::vector<Point> sortedCentres;
Point topLeft;
Point bottomRight;

std::vector<Cell> trayCells;

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

void Vision::cellAverages() {
	std::vector<std::vector<int>> cellCoordinates;
	std::vector<int> xCoords;
	std::vector<int> yCoords;
	std::vector<Point> points;
	double cellWidth;
	double cellHeight;


	namedWindow("Contours", WINDOW_NORMAL);
	imshow("Contours", drawing);
	resizeWindow("Contours", 1600, 800);
	setMouseCallback("Contours", onMouse, &cellList);
	waitKey(0);

	std::vector<int> xList;
	std::vector<int> yList;
	Cell lastCell = cellList[0];

	for (int i = 0; i < cellList.size(); i++) {
		if (cellList[i].getCellCoordinateX() > lastCell.getCellCoordinateX()) {
			xList.push_back(cellList[i].getCellCoordinateX() - lastCell.getCellCoordinateX());
		}
		else {
			yList.push_back(cellList[i].getCellCoordinateY() - lastCell.getCellCoordinateY());
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

void Vision::initialiseTrays() {
	Point start;
	Point end;

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
}

void Vision::colourMask() {
	assert(!image.empty());

	// Converts image from BGR to HSV
	cvtColor(image, imageHSV, COLOR_BGR2HSV);

	// Detects yellow colour
	inRange(imageHSV, Scalar(20, 100, 100), Scalar(30, 255, 255), trayImage);
}

void Vision::contourDetection() {
	// Canny edge detection
	Mat cannyOutput;
	Canny(trayImage, cannyOutput, 50, 150, 3);
	// Gaussian blur
	Mat blur;
	GaussianBlur(cannyOutput, blur, Size(3, 3), 0);
	// Find contours
	findContours(blur, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// Sort contours
	sort(contours.begin(), contours.end(), [](const std::vector<Point>& c1, const std::vector<Point>& c2) {
		return contourArea(c1, false) < contourArea(c2, false);
		});
}

void Vision::drawContourCentres() {
	// Draw largest contour and centre circle
	if (contours.size() < 1) {
		std::cout << "First dot not detected" << std::endl;
		return;
	}
	// Maximum and minimum x and y coordinates
	for (int i = 0; i < dotNumber; i++) {
		drawContours(drawing, contours, (i), Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point());
		Moments m = moments(contours[i], true);
		Point p(m.m10 / m.m00, m.m01 / m.m00);
		dotCentres.push_back(p);
		circle(drawing, p, 5, Scalar(0, 0, 255), -1);
	}
}

void Vision::sortDots() {
	std::vector<Point> sortedPoints;
	sortedPoints = dotCentres;
	std::sort(sortedPoints.begin(), sortedPoints.end(), [](Point a, Point b) {
		return a.x < b.x;
		});
	topLeft = sortedPoints[0];
	bottomRight = sortedPoints[1];
	std::cout << "topLeft x: " << topLeft.x << std::endl;
	std::cout << "topLeft y: " << topLeft.y << std::endl;
	std::cout << "bottomRight x: " << bottomRight.x << std::endl;
	std::cout << "bottomRight y: " << bottomRight.y << std::endl;
}

void Vision::drawCells() {
	Mat imageCopy = image.clone();
	std::vector<Point> cellCorners;
	int trayWidth = bottomRight.x - topLeft.x;
	std::cout << "Tray width: " << trayWidth << std::endl;
	int trayHeight = bottomRight.y - topLeft.y;
	std::cout << "Tray height: " << trayHeight << std::endl;
	for (int i = 0; i < 4; i++) {
		int cellY = topLeft.y + (trayHeight / 3 * i);
		for (int j = 0; j < 5; j++) {
			int cellX = topLeft.x + (trayWidth / 4 * j);
			cellCorners.push_back(Point(cellX, cellY));
			if (i != 3 && j != 4) {
				trayCells.push_back(Cell(cellX, cellY));
			}
		}
	}
	for (int i = 0; i < traySize - 7; i++) {
		rectangle(imageCopy, cellCorners[i], cellCorners[i + 6], Scalar(0, 0, 255), 6, 8, 0);
	}
	for (int i = 0; i < trayCells.size(); i++) {
		circle(imageCopy, Point(trayCells[i].getCellCoordinateX(), trayCells[i].getCellCoordinateY()), 12, Scalar(255, 0, 0), -1);
	}
	namedWindow("Cells", WINDOW_NORMAL);
	imshow("Cells", imageCopy);
	resizeWindow("Cells", 1600, 800);
	waitKey(0);
}

void Vision::trayDetection(std::string filename) {
	image = imread(filename, IMREAD_COLOR);
	drawing = image.clone();

	colourMask(); // Converts image to hsv and adds yellow mask
	contourDetection(); // Finds contours
}

void Vision::calculateCellMeasurements(std::string filename, int tray, int dots) {
	image = imread(filename, IMREAD_COLOR);
	drawing = image.clone();
	traySize = tray;
	dotNumber = dots;

	colourMask(); // Converts image to hsv and adds yellow mask
	contourDetection(); // Finds contours
	drawContourCentres(); // Draws contour centres
	cellAverages(); // Calculates cell measurement averages
}

void Vision::initialiseCells(std::string filename, int tray, int dots) {
	image = imread(filename, IMREAD_COLOR);
	drawing = image.clone();
	traySize = tray;
	dotNumber = dots;

	colourMask(); // Converts image to hsv and adds yellow mask
	contourDetection(); // Finds contours
	drawContourCentres(); // Draws contour centres
	sortDots();
	drawCells();
}

int main()
{
	Vision vision;
	vision.initialiseCells("two_dots.jpg", 20, 2);
}
