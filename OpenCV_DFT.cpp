#include<opencv2/opencv.hpp>
#include<iostream>
#include <stdint.h>

using namespace cv;

int main(int argv, char** argc)
{
	// Initial image
	Mat original = imread("image.png", IMREAD_GRAYSCALE);

	// Mat for converting image matrix into floats
	Mat originalFloat;

	// Converts original image matrix into floats
	original.convertTo(originalFloat, CV_32FC1, 1.0 / 255.0);

	// Creates an array of matrices consisting of originalFloat and a matrix full of zeroes
	Mat originalComplex[2] = { originalFloat, Mat::zeros(originalFloat.size(), CV_32F) };

	// Matrix for the dft
	Mat dftReady;

	// Merges the array of originalComplex with the matrix dftReady
	merge(originalComplex, 2, dftReady);

	// Matrix for the dft
	Mat dftOfOriginal;

	// Creates a dft in dftReady out of dftOfOriginal with complex numbers
	dft(dftReady, dftOfOriginal, DFT_COMPLEX_OUTPUT);
}