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

/*
void Vision::dotDetection(Tray tray, cv::Mat image, cv::Mat& input) {
	Mat originalImage = input.clone();
	Moments m = moments(image, true);
	Point p(m.m10 / m.m00, m.m01 / m.m00);
	std::vector<double> coordinates = realPosition(p.x, p.y);
	tray.setCoordinates(coordinates);
	tray.printCoordinates();

	// Dot display
	namedWindow("Detection Dot Centre", WINDOW_NORMAL);
	circle(originalImage, p, 5, Scalar(0, 255, 0), -1);
	imshow("Detection Dot Centre", originalImage);
	resizeWindow("Detection Dot Centre", 1600, 800);
	waitKey(0);
	destroyWindow("Detection Dot Centre");
}
*/

void Vision::dotDetection(Tray tray, cv::Mat image) {
	Mat cannyOutput;
	// Canny edge detection
	Canny(image, cannyOutput, 50, 150, 3);
	// Find contours
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	findContours(cannyOutput, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// get the moments
	std::vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	// get the centroid of figures.
	std::vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	// draw contours
	Mat drawing(cannyOutput.size(), CV_8UC3, Scalar(255, 255, 255));
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar colour = Scalar(167, 151, 0); // B G R values
		Scalar colourCentre = Scalar(0, 0, 255);
		double area = contourArea(contours.at(i));
		if (area > 20) {
			drawContours(drawing, contours, i, colour, 2, 8, hierarchy, 0, Point());
			circle(drawing, mc[i], 4, colourCentre, -1, 8, 0);
		}
	}
	// show the resultant image
	namedWindow("Contours", WINDOW_NORMAL);
	imshow("Contours", drawing);
	resizeWindow("Contours", 1600, 800);
	waitKey(0);
}

void Vision::maskWindowsContour(const Mat& inputBGRimage) {
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

	// Red Mask
	inRange(imageHSV, Scalar(0, 70, 50), Scalar(10, 255, 255), imgRed1);
	inRange(imageHSV, Scalar(170, 70, 50), Scalar(180, 255, 255), imgRed2);
	imgRed = imgRed1 | imgRed2;
	dotDetection(trayRed, imgRed);

	/*
// Image display
namedWindow("Red Mask", WINDOW_NORMAL);
imshow("Red Mask", imgRedBlur);
resizeWindow("Red Mask", 1600, 800);
waitKey(0);
std::cout << "Red Tray Detection" << std::endl;
// Dot detection using blurred mask
// dotDetection(trayRed, imgRedBlur, input);
*/

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
	maskWindowsContour(image);
}

int main()
{
	Vision vision;
	vision.trayDetection("tray_dots.jpg");
}
