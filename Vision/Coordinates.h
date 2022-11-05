#pragma once
#include <utility>

class Coordinates {
private:
	static constexpr double fx = 276.558;
	static constexpr double fy = 1439.6;
	static constexpr double cx = 2.49999;
	static constexpr double cy = 3.99997;
	static constexpr double xDimensionTray = 18.2;
	static constexpr double yDimensionTray = 14.2;
	static constexpr double cameraHeight = 22.5;
public:
	inline static std::pair<int, int> realCoordinates(int x, int y);
};