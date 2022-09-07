#include "Cell.h"
#include <vector>
#include <iostream>

int xCellCoordinate;
int yCellCoordinate;

Cell::Cell(int x, int y) {
	xCellCoordinate = x;
	yCellCoordinate = y;
}

int Cell::getCellCoordinateX() {
	return xCellCoordinate;
}

int Cell::getCellCoordinateY() {
	return yCellCoordinate;
}