#pragma once
class Tray {
private:
	int xCoordinate;
	int yCoordinate;
public:
	Tray();
	Tray(int x, int y);
	void setCoordinates(int x, int y);
	void printCoordinates();
};

