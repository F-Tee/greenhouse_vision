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
	cv::Mat input = inputBGRimage.clone();
	cv::Mat imageHSV;
	cv::Mat imgThreshold, imgThreshold0, imgThreshold1;
	assert(!input.empty());

	cv::cvtColor(input, imageHSV, cv::COLOR_BGR2HSV);

	cv::inRange(imageHSV, cv::Scalar(0, 53, 185, 0), cv::Scalar(rng, 255, 255, 0), imgThreshold0);

	if (rng > 0)
	{
		cv::inRange(imageHSV, cv::Scalar(180 - rng, 53, 185, 0), cv::Scalar(180, 255, 255, 0), imgThreshold1);
		cv::bitwise_or(imgThreshold0, imgThreshold1, imgThreshold);
	}
	else
	{
		imgThreshold = imgThreshold0;
	}

	return imgThreshold;
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

	Mat image = imread("tray_test.png", IMREAD_COLOR);

	Mat foundColour = findColour(image);

	imshow("Find Colour - Red", foundColour);

	waitKey();

	return 0;

}