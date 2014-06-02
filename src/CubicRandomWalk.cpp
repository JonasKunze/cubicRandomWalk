//============================================================================
// Name        : CubicRandomWalk.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdlib.h>
#include <time.h>

#include "simple_svg_1.0.0_Kunze_0.1.hpp"
using namespace svg;

struct Pos {
	int x;
	int y;
};

int calculateRee(Pos* steps, int N) {
	int endX = steps[N - 1].x;
	int endY = steps[N - 1].y;

	return endX * endX + endY * endY;
}

void drawSvg(Pos* steps, int N) {
	std::string fileName = "randomWalk.svg";
	float scale = 20; // scale factor for better presentation in chrome

	/*
	 * Find maximum values of the walk to center the image
	 */
	float xmin = 0, xmax = 0;
	float ymin = 0, ymax = 0;

	for (int stepNum = 0; stepNum < N; stepNum++) {
		int x = steps[stepNum].x;
		int y = steps[stepNum].y;

		xmin = xmin > x ? x : xmin;
		ymin = ymin > y ? y : ymin;

		xmax = xmax < x ? x : xmax;
		ymax = ymax < y ? y : ymax;
	}

	int width = xmax - xmin;
	int height = ymax - ymin;

	float margin = 1;
	Dimensions dimensions(scale * (width + margin * 2),
			scale * (height + margin * 2));
	float xshift = -xmin + margin;
	float yshift = -ymin + margin;

	Document doc(fileName, Layout(dimensions, Layout::BottomLeft, scale));

	Polyline polyline(Stroke(.1, Color::Blue));

	for (int stepNum = 0; stepNum < N; stepNum++) {
		polyline << Point(steps[stepNum].x + xshift, steps[stepNum].y + yshift);
	}

	doc << polyline;

	doc << Circle(Point(xshift, yshift), 0.6, Fill(Color(255, 0, 0)));

	doc
			<< Circle(Point(steps[N - 1].x + xshift, steps[N - 1].y + yshift),
					0.5, Fill(Color(0, 255, 0)));

	doc.save();

	std::cout << fileName << " was successfully written." << std::endl;
}

/**
 * Generates a walk (array of Pos objects) with <numberOfSteps> steps (Pos)
 */
Pos* generateWalk(int numberOfSteps) {
	Pos* steps = new Pos[numberOfSteps];

	Pos currentPos = { 0, 0 };

	/*
	 * First point is (0,0)
	 */
	steps[0].x = currentPos.x;
	steps[0].y = currentPos.y;

	for (int stepNum = 1; stepNum < numberOfSteps; stepNum++) {
		int direction = rand() % 4;

		if (direction == 0) { // North
			currentPos.y++;
		} else if (direction == 1) { // East
			currentPos.x++;
		} else if (direction == 2) { // South
			currentPos.y--;
		} else if (direction == 3) { // West
			currentPos.x--;
		}
		steps[stepNum].x = currentPos.x;
		steps[stepNum].y = currentPos.y;
	}
	return steps;
}

int main() {
	// Set seed for later rand useage
	srand(time(NULL));

	int numberOfStepsPerWalk = 100, numberOfWalks = 100;

	std::cout << "Please enter the number of steps per walk: ";
	std::cin >> numberOfStepsPerWalk;

	std::cout << "Please enter the number of walks to be simulated: ";
	std::cin >> numberOfWalks;

	int reeSum = 0;

	/*
	 * Calculate <numberOfWalks> walks with <numberOfStepsPerWalk> steps each
	 */
	for (int walk = 0; walk < numberOfWalks; walk++) {
		Pos* steps = generateWalk(numberOfStepsPerWalk);
		int ree = calculateRee(steps, numberOfStepsPerWalk);
		reeSum += ree;

		float averageRee = reeSum / (float) (walk + 1);

		std::cout << "Walk " << walk << ": Ree^2 = " << ree << " (avg. "
				<< averageRee << ")" << std::endl;

		delete[] steps;
	}

	Pos* steps = generateWalk(numberOfStepsPerWalk);
	drawSvg(steps, numberOfStepsPerWalk);
	delete[] steps;

	return 0;
}
