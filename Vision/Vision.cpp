#include "Vision.h"
#include "Cell.h"
#include "Tray.h"
#include "Coordinates.h"
#include "Coordinates.cpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/aruco.hpp"
#include <armadillo.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdint.h>

using namespace cv;


const int fps = 20;

const int cellDistanceX = 574;
const int cellDistanceY = 574;

int pixelTrayWidth;
int numberOfTrays;

Mat image;
Mat imageHSV;
Mat drawing;

Mat trayImage;
std::vector<Tray> trays;

std::vector<std::vector<Point>> contours;
std::vector<Vec4i> hierarchy;

std::vector<Cell> cellList;
int xAvg;
int yAvg;

std::vector<Point> dotCentres;
std::vector<Point> sortedCentres;

std::vector<Cell> trayCells;

Vision::Vision(int trayQuantity) {
	numberOfTrays = trayQuantity;
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

void Vision::initialiseTrays() {
	// Loop through number of trays * dots (2 per tray)
	// Each two dots change tray
	// Add tray to trays vector with top left dot (lower value centroid) 
	// as cell coordinate 
	// Change this to add all four corners?
	std::cout << "dotCentres size: " << dotCentres.size() << std::endl;
	std::vector<int> previousCoordinates;
	std::vector<Point> euclid;
	for (int i = 0; i < dotCentres.size(); i++) {
		std::vector<int> dotCoords = { dotCentres[i].x, dotCentres[i].y };
		if (i % 2 != 0) {
			Point point = Point(dotCoords[0], dotCoords[1]);
			trays.push_back(Tray(dotCoords[0], dotCoords[1], previousCoordinates[0], previousCoordinates[1]));
			std::cout << "Point created" << std::endl;
			euclid.push_back(point);
			std::cout << "Point pushed" << std::endl;
		}
		else {
			previousCoordinates = dotCoords;
		}
	}
	std::cout << "Beginning euclidian distance test" << std::endl;
	euclidian_distance_test(euclid);
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
	for (int i = 0; i < (numberOfTrays * 2); i++) {
		drawContours(drawing, contours, (i), Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point());
		Moments m = moments(contours[i], true);
		Point p(m.m10 / m.m00, m.m01 / m.m00);
		dotCentres.push_back(p);
		circle(drawing, p, 5, Scalar(0, 0, 255), -1);
	}
}

void Vision::sortDots() {
	std::sort(dotCentres.begin(), dotCentres.end(), [](Point a, Point b) {
		return a.x > b.x;
		});
}

void Vision::drawCells() {
	Mat imageCopy = image.clone();
	for (int i = 0; i < trays.size(); i++) {
		std::vector<Point> trayCorners = trays[i].getCorners();
		std::vector<Point> cellCorners;
		std::cout << "Top left corner x: " << trayCorners[0].x << std::endl;
		std::cout << "Top left corner y: " << trayCorners[0].y << std::endl;
		std::cout << "Bottom right corner x: " << trayCorners[1].x << std::endl;
		std::cout << "Bottom right corner y: " << trayCorners[1].y << std::endl;
		pixelTrayWidth = trayCorners[1].x - trayCorners[0].x;
		std::cout << "Tray width: " << pixelTrayWidth << std::endl;
		int trayHeight = trayCorners[1].y - trayCorners[0].y;
		std::cout << "Tray height: " << trayHeight << std::endl;
		for (int i = 0; i < 4; i++) {
			int cellY = trayCorners[0].y + (trayHeight / 3 * i);
			for (int j = 0; j < 5; j++) {
				int cellX = trayCorners[0].x + (pixelTrayWidth / 4 * j);
				cellCorners.push_back(Point(cellX, cellY));
				if (i != 3 && j != 4) {
					std::cout << "x: " << cellX << " y: " << cellY << std::endl;
					std::pair<int, int> realCoords = Coordinates::realCoordinates(cellX, cellY);
					std::cout << "realX: " << realCoords.first << " realY: " << realCoords.second << std::endl;
					trayCells.push_back(Cell(cellX, cellY));
				}
			}
		}
		for (int i = 0; i < 13; i++) {
			rectangle(imageCopy, cellCorners[i], cellCorners[i + 6], Scalar(0, 0, 255), 6, 8, 0);
		}
		for (int i = 0; i < trayCells.size(); i++) {
			circle(imageCopy, Point(trayCells[i].getCellCoordinateX(), trayCells[i].getCellCoordinateY()), 12, Scalar(255, 0, 0), -1);
		}
	}
	namedWindow("Cells", WINDOW_NORMAL);
	imshow("Cells", imageCopy);
	resizeWindow("Cells", 1600, 800);
	waitKey(0);
}

double Vision::euclidian_distance_test(std::vector<cv::Point> vector) {
	std::cout << "Euclidian distance started" << std::endl;
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	std::cout << vector.size() << std::endl;
	x1 = vector[0].x;
	y1 = vector[0].y;
	x2 = vector[1].x;
	y2 = vector[1].y;
	std::pair<int, int> pair1 = Coordinates::realCoordinates(x1, y1);
	x1 = pair1.first;
	y1 = pair1.second;
	std::pair<int, int> pair2 = Coordinates::realCoordinates(x2, y2);
	x2 = pair2.first;
	y2 = pair2.second;
	double sum = pow((x2 - x1), 2) + pow((y2 - y1), 2);
	double result = sqrt(sum);
	std::cout << "Euclidian distance: " << result << std::endl;
	return result;
}

void drawCentre() {
	Mat imageCopy = image.clone();
	Point p(249, 800);
	circle(imageCopy, p, 12, Scalar(0, 0, 255), -1);
	namedWindow("Centre", WINDOW_NORMAL);
	imshow("Centre", imageCopy);
	resizeWindow("Centre", 1600, 800);
	waitKey(0);
}

void Vision::trayDetection(std::string filename) {
	image = imread(filename, IMREAD_COLOR);
	drawing = image.clone();

	colourMask(); // Converts image to hsv and adds yellow mask
	contourDetection(); // Finds contours
}

void Vision::calculateCellMeasurements(std::string filename) {
	image = imread(filename, IMREAD_COLOR);
	drawing = image.clone();

	colourMask(); // Converts image to hsv and adds yellow mask
	contourDetection(); // Finds contours
	drawContourCentres(); // Draws contour centres
	cellAverages(); // Calculates cell measurement averages
}

void Vision::initialiseCells(std::string filename) {
	image = imread(filename, IMREAD_COLOR);
	drawing = image.clone();

	colourMask(); // Converts image to hsv and adds yellow mask
	contourDetection(); // Finds contours
	drawContourCentres(); // Draws contour centres
	sortDots();
	initialiseTrays();
	drawCells();
	drawCentre();
}

int main()
{
	Vision vision(3);
	vision.initialiseCells("three_trays.jpg");
}
