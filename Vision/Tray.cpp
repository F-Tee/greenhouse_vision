#include "Tray.h"
#include <vector>
#include <iostream>
#include "Cell.h"

int xCoordinate;
int yCoordinate;
bool initialised = false;
std::vector<Cell> cells;

Tray::Tray() {
	xCoordinate = 0;
	yCoordinate = 0;
}

Tray::Tray(int x, int y) {
	xCoordinate = x;
	yCoordinate = y;
	initialised = true;
}

void Tray::setCoordinates(std::vector<double> coordinates) {
	xCoordinate = coordinates.at(0);
	yCoordinate = coordinates.at(1);
	initialised = true;
}

void Tray::printCoordinates() {
	std::cout << "Real position:" << std::endl;
	std::cout << "x: " << xCoordinate << std::endl;
	std::cout << "y: " << yCoordinate << std::endl;
	std::cout << std::endl;
}

bool Tray::getInitialised() {
	return initialised;
}

void Tray::initialiseCells() {
	while (cells.size() < 12) {
		cells.push_back(Cell());
	}
}