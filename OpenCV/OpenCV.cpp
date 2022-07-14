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

const float calibrationSquareDimension = 0.0241f; // metres
const Size chessboardDimensions = Size(6, 9);

void createKnownBoardPosition(Size boardSize, float squareEdgeLength, std::vector<Point3f>& corners) {
	for (int i = 0; i < boardSize.height; i++) {
		for (int j = 0; j < boardSize.width; j++) {
			corners.push_back(Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
		}
	}
}

void getChessboardCorners(std::vector<Mat> images, std::vector<std::vector<Point2f>>& allFoundCorners, bool showResults) {
	for (std::vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++) {
		std::vector<Point2f> pointBuf;
		bool found = findChessboardCorners(*iter, Size(9, 6), pointBuf, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);

		if (found) {
			allFoundCorners.push_back(pointBuf);
		}

		if (showResults) {
			drawChessboardCorners(*iter, Size(9, 6), pointBuf, found);
			imshow("Looking for Corners", *iter);
			waitKey(0);
		}
	}
}

void createArucoMarkers() {
	Mat outputMarker;

	// Sets up aruco marker dictionary
	Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

	// Iterates over markers in dictionary
	for (int i = 0; i < 50; i++) {
		// Draws an aruco marker using the marker specified by i in the dictionary
		aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
		// Creates an output string stream for the filename
		std::ostringstream convert;
		// String for image name
		std::string imageName = "4x4Marker_";
		// Concatenates the imageName with the number of the iteration as well as the filetype
		convert << imageName << i << ".jpg";
		// Writes the marker with the right filename
		imwrite(convert.str(), outputMarker);
	}
}

void cameraCalibration(std::vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat& distanceCoefficients) {
	std::vector<std::vector<Point2f>> checkerboardImageSpacePoints;
	getChessboardCorners(calibrationImages, checkerboardImageSpacePoints, false);

	std::vector<std::vector<Point3f>> worldSpaceCornerPoints(1);

	createKnownBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
	worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

	std::vector<Mat> rVectors, tVectors;
	distanceCoefficients = Mat::zeros(8, 1, CV_64F);

	calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoints, boardSize, cameraMatrix, distanceCoefficients, rVectors, tVectors);
}

bool saveCameraCalibration(std::string name, Mat cameraMatrix, Mat distanceCoefficients) {
	std::ofstream outStream(name);
	if (outStream) {
		uint16_t rows = cameraMatrix.rows;
		uint16_t columns = cameraMatrix.cols;

		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < columns; c++) {
				double value = cameraMatrix.at<double>(r, c);
				outStream << value << std::endl;
			}
		}

		rows = distanceCoefficients.rows;
		columns = distanceCoefficients.cols;

		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < columns; c++) {
				double value = distanceCoefficients.at<double>(r, c);
				outStream << value << std::endl;
			}
		}

		outStream.close();
		return true;
	}

	return false;
}

int main(int argv, char** argc)
{
	Mat frame;
	Mat drawToFrame;

	Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

	Mat distanceCoefficients;

	std::vector<Mat> savedImages;

	std::vector<std::vector<Point2f>> markerCorners, rejectedCandidates;

	VideoCapture vid(0);

	if (!vid.isOpened()) {
		return 0;
	}

	int framesPerSecond = 20;

	namedWindow("Webcam", WINDOW_AUTOSIZE);

	while (true) {
		if (!vid.read(frame)) {
			break;
		}

		std::vector<Vec2f> foundPoints;
		bool found = false;

		found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FAST_CHECK);
		frame.copyTo(drawToFrame);
		drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
		if (found) {
			imshow("Webcam", drawToFrame);
		}
		else {
			imshow("Webcam", frame);
		}
		char character = waitKey(1000 / framesPerSecond);

		switch (character) {
		case ' ':
			// space
			// saving image
			if (found) {
				Mat temp;
				frame.copyTo(temp);
				savedImages.push_back(temp);
			}
			break;
		case 13:
			// enter key
			// start calib
			if (savedImages.size() > 15) {
				cameraCalibration(savedImages, chessboardDimensions, calibrationSquareDimension, cameraMatrix, distanceCoefficients);
				saveCameraCalibration("Camera_Calibration", cameraMatrix, distanceCoefficients);
			}
			break;
		case 27:
			// exit
			return 0;
			break;
		}
	}

	return 0;
}