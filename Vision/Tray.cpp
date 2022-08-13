#include "Tray.h"
#include <iostream>

int xCoordinate;
int yCoordinate;

Tray::Tray() {
	xCoordinate = 0;
	yCoordinate = 0;
}

Tray::Tray(int x, int y) {
	xCoordinate = x;
	yCoordinate = y;
}

void Tray::setCoordinates(int x, int y) {
	xCoordinate = x;
	yCoordinate = y;
}

void Tray::printCoordinates() {
	std::cout << "x coordinate: " << xCoordinate << std::endl;
	std::cout << "y coordinate: " << yCoordinate << std::endl;
}