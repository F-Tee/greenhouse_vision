#pragma once
class Cell
{
private:
	int xCellCoordinate;
	int yCellCoordinate;
public:
	Cell(double x, double y);

	double getCellCoordinateX();

	double getCellCoordinateY();
};

