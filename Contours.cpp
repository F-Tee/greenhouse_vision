#include<opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/aruco.hpp"


#include<iostream>
#include <sstream>
#include <fstream>
#include <stdint.h>

using namespace cv;

const int fps = 20;

cv::Mat findColour(const cv::Mat& inputBGRimage, int rng = 15)
{
	Mat input = inputBGRimage.clone();
	Mat imageHSV;
	Mat imgThreshold;
	assert(!input.empty());

	// Converts image from BGR to HSV
	cvtColor(input, imageHSV, COLOR_BGR2HSV);

	// Detects yellow colour
	inRange(imageHSV, cv::Scalar(20, 55, 55), Scalar(30, 255, 255), imgThreshold);

	return imgThreshold;
}

void contoursFromColour(std::string filename) {
	Mat image = imread(filename, IMREAD_COLOR);
	Mat thresh;

	thresh = findColour(image);

	// detect edges using canny
	Mat canny;
	Canny(thresh, canny, 50, 150, 3);

	// Detect contours on binary threshold image
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	findContours(canny, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);

	Mat image_copy = image.clone();
	drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);

	// Show image
	imshow("Contours", image_copy);
	waitKey(0);
}

void trayContoursThreshold(const cv::Mat& inputBGRimage) {
	Mat input = inputBGRimage.clone();
	Mat imageGrey;
	assert(!input.empty());

	// Convert image to greyscale
	cvtColor(input, imageGrey, COLOR_BGR2GRAY);

	// Setting binary threshold
	// Mat thresh;
	// threshold(imageGrey, thresh, 80, 255, THRESH_BINARY);

	// Blur image
	Mat imageBlurred;
	GaussianBlur(imageGrey, imageBlurred, Size(3, 3), 0);

	// Otsu's threshold
	// Mat otsu;
	// threshold(imageBlurred, otsu, 0, 255, THRESH_BINARY | THRESH_OTSU);

	// Detect edges using canny
	Mat canny;
	Canny(imageBlurred, canny, 70, 255, 3, true);

	// Detect contours on binary threshold image
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	findContours(canny, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	// Draw contours
	Mat image_copy = input.clone();
	drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);

	/*
	if (!contours.empty() && !hierarchy.empty()) {

		// loop through the contours/hierarchy
		for (int i = 0; i < contours.size(); i++) {

			// look for hierarchy[i][3]!=-1, ie hole boundaries
			if (hierarchy[i][3] != -1) {
				drawContours(input, contours, i, Scalar(0, 0, 255));
			}
		}
	}
	*/

	// Show image
	imshow("Contours", image_copy);
	waitKey(0);
}

void findContoursTray(std::string filename) {
	Mat image = imread(filename, IMREAD_COLOR);

	Mat foundContours;
	trayContoursThreshold(image);

}

int main(int argv, char** argc) {
	/*
	Mat frame;
	Mat temp;
	Mat savedImage;

	VideoCapture vid(0);

	if (!vid.isOpened()) {
		return 0;
	}

	while (true) {
		vid.read(frame);
		imshow("Webcam", frame);

		char character = waitKey(1000 / fps);

		if (character == ' ') {
			Mat temp;
			frame.copyTo(temp);
			savedImage = temp;
		}
		else {
			return 0;
			break;
		}
	}
	*/

	findContoursTray("tray_test_photo2.jpg");

	return 0;

}