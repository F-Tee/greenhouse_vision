#include "Cell.h"

#include <vector>
#include <iostream>

int xCellCoordinate;
int yCellCoordinate;

Cell::Cell(double x, double y) {
	xCellCoordinate = x;
	yCellCoordinate = y;
}

double Cell::getCellCoordinateX() {
	return xCellCoordinate;
}

double Cell::getCellCoordinateY() {
	return yCellCoordinate;
}