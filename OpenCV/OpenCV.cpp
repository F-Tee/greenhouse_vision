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

int main(int argv, char** argc)
{
	createArucoMarkers();
}