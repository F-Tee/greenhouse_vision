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

static Mat imgRed;

void Vision::onMouse(int event, int x, int y, int flags, void* param)
{
	if (event != EVENT_LBUTTONDOWN)
	{
		return;
	}
	Tray* pThis = (Tray*)param;
	pThis->setCoordinates(x, y);
	pThis->printCoordinates();
}

void Vision::maskWindows(const Mat& inputBGRimage) {
	Mat input = inputBGRimage.clone();
	Mat imageHSV;
	Mat imgRed1, imgRed2;
	Mat imgGreen;
	Mat imgBlue;
	Tray tray1;
	assert(!input.empty());

	// Converts image from BGR to HSV
	cvtColor(input, imageHSV, COLOR_BGR2HSV);

	// Detects red colour
	inRange(imageHSV, Scalar(0, 70, 50), Scalar(10, 255, 255), imgRed1);
	inRange(imageHSV, Scalar(170, 70, 50), Scalar(180, 255, 255), imgRed2);
	imgRed = imgRed1 | imgRed2;
	namedWindow("Red Detection");
	setMouseCallback("Red Detection", onMouse, &tray1);
	imshow("Red Detection", imgRed);
	waitKey(0);

	// Detects green colour
	inRange(imageHSV, Scalar(38, 0, 0), Scalar(70, 255, 255), imgGreen);

	// Detects blue colour
	inRange(imageHSV, Scalar(110, 50, 50), Scalar(130, 255, 255), imgBlue);
}

void Vision::trayDetection(std::string filename) {
	Mat image = imread(filename, IMREAD_COLOR);

	maskWindows(image);

}

int main()
{
	Vision vision;
	vision.trayDetection("tray_test2.png");
}
