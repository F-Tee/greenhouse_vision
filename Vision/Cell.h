#pragma once
class Cell
{
private:
	int xCellCoordinate;
	int yCellCoordinate;
public:
	Cell(int x, int y);

	int getCellCoordinateX();

	int getCellCoordinateY();
};

