#pragma once

#include "Tray.h"
#include "Cell.h"
#include <vector>

class Tray {
private:
	int xCoordinate;
	int yCoordinate;
	std::vector<Cell> cells;
public:
	Tray();
	Tray(int x, int y);
	void setCoordinates(std::vector<int> coordinates);
	void printCoordinates();
};

