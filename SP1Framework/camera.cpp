#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>

int cameraPosition[2] = {0, 0};
int cameraSize[2] = {50, 50};
extern COORD charLocation;
extern Console console;

//2D array to store the position of the walls and items etc.
extern unsigned int g_worldGrid[worldSizeX][worldSizeY];
char worldBlocks[10] = {178, 0};

void moveCameraTo() {

	if (charLocation.X > 20) {
		cameraPosition[0] = charLocation.X - 20;
	} else {
		cameraPosition[0] = 0;
	}
		
	if (charLocation.Y > 20) {
		cameraPosition[1] = charLocation.Y - 20;
	} else {
		cameraPosition[1] = 0;
	}

}

void renderView() {

	for (unsigned int x = 0; x < cameraSize[0]; ++x) {

		for (unsigned int y = 0; y < cameraSize[1]; ++y) {

			if (g_worldGrid[cameraPosition[0] + x][cameraPosition[1] + y] == 1) {

				console.writeToBuffer(x, y, worldBlocks[0], 0x0D);

			}

		}

	}

}