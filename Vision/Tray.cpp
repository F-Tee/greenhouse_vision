#include "Tray.h"
#include "Cell.h"
#include <vector>
#include <iostream>

int xCoordinate;
int yCoordinate;
std::vector<Cell> cells;

Tray::Tray() {
	xCoordinate = 0;
	yCoordinate = 0;
}

Tray::Tray(int x, int y) {
	xCoordinate = x;
	yCoordinate = y;
}

void Tray::setCoordinates(std::vector<int> coordinates) {
	xCoordinate = coordinates.at(0);
	yCoordinate = coordinates.at(1);
}

void Tray::printCoordinates() {
	std::cout << "Real position:" << std::endl;
	std::cout << "x: " << xCoordinate << std::endl;
	std::cout << "y: " << yCoordinate << std::endl;
	std::cout << std::endl;
}