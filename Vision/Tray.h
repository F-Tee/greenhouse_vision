#pragma once

#include <vector>

class Tray {
private:
	int xCoordinate;
	int yCoordinate;
	bool initialised;
public:
	Tray();
	Tray(int x, int y);
	void setCoordinates(std::vector<double> coordinates);
	void printCoordinates();
	bool getInitialised();
	void initialiseCells();
};

