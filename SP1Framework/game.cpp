// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Console object
Console console(80, 25, "The Mummy");

double elapsedTime;
double deltaTime;
bool keyPressed[K_COUNT];

// Game specific variables here
COORD charLocation;

//Player Body Parts
char g_playerHead[4] = {'\\', (char)1, '/', '\0'};
char g_playerBody[4] = {' ', '|', ' ', '\0'};
char g_playerLegs[4] = {'/', ' ', '\\', '\0'};

//Jumping and Gravity
char g_jumpHeight = 0;
bool g_onGround = true;

//Size of X and Y of console *NOTE: consoleSizeX and consoleSizeY is made by me(Terry) and is different from consoleSize.X and consoleSize.Y which is made by Mr Wee.
//Edit the values of consoleSizeX and consoleSizeY to change the size of the console.
const unsigned int consoleSizeX = 150;
const unsigned int consoleSizeY = 50;

//2D array to store the position of the walls and items etc.
const unsigned int worldSizeX = 600;
const unsigned int worldSizeY = 200;
unsigned int g_worldGrid[worldSizeX][worldSizeY] = {0};

// Initialize variables, allocate memory, load data from file, etc. 
// This is called once before entering into your main loop
void init()
{
    // Set precision for floating point output
    elapsedTime = 0.0;

    charLocation.X = console.getConsoleSize().X / 2;
    charLocation.Y = console.getConsoleSize().Y / 2;
    // sets the width, height and the font name to use in the console
    console.setConsoleFont(0, 16, L"Consolas");
}

// Do your clean up of memory here
// This is called once just before the game exits
void shutdown()
{
    // Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    console.clearBuffer();
}
/*
	This function checks if any key had been pressed since the last time we checked
	If a key is pressed, the value for that particular key will be true
	
	Add more keys to the enum in game.h if you need to detect more keys
	To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
	For Alphanumeric keys, the values are their ascii values (uppercase).
*/
void getInput()
{    
    keyPressed[K_UP] = isKeyPressed(VK_UP);
    keyPressed[K_DOWN] = isKeyPressed(VK_DOWN);
    keyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
    keyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
    keyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
}

/*
	This is the update function
	double dt - This is the amount of time in seconds since the previous call was made

	Game logic should be done here.
	Such as collision checks, determining the position of your game characters, status updates, etc
	If there are any calls to write to the console here, then you are doing it wrong.

    If your game has multiple states, you should determine the current state, and call the relevant function here.
*/
void update(double dt)
{
    // get the delta time
    elapsedTime += dt;
    deltaTime = dt;

    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
    // sound can be played here too.
}

/*
    This is the render loop
    At this point, you should know exactly what to draw onto the screen.
    Just draw it!
    To get an idea of the values for colours, look at console.h and the URL listed there
*/
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void moveCharacter()
{
    // Updating the location of the character based on the key press
	if (keyPressed[K_UP] && charLocation.Y > 0 && g_worldGrid[charLocation.X][charLocation.Y - 3] == 0 && g_worldGrid[charLocation.X - 1][charLocation.Y - 3] == 0 && g_worldGrid[charLocation.X + 1][charLocation.Y - 3] == 0 && g_onGround == true) {
        g_jumpHeight = 6;
    }

	if (g_worldGrid[charLocation.X][charLocation.Y - 3] != 0) {
		g_jumpHeight = 0;
	}

	if (g_jumpHeight > 0) {
		
		--charLocation.Y;
		--g_jumpHeight;
		g_onGround = false;

	} else if (g_worldGrid[charLocation.X][charLocation.Y + 1] == 0) {
		
		++charLocation.Y;

	} else if (g_worldGrid[charLocation.X][charLocation.Y + 1] != 0) {
		
		g_onGround = true;

	}

    if (keyPressed[K_LEFT] && charLocation.X > 0 && g_worldGrid[charLocation.X - 2][charLocation.Y] == 0 && g_worldGrid[charLocation.X - 2][charLocation.Y - 1] == 0 && g_worldGrid[charLocation.X - 2][charLocation.Y - 2] == 0) {
		charLocation.X--;
		g_playerHead[0] = ' ';
		g_playerHead[1] = '@';
		g_playerHead[2] = ' ';
		g_playerHead[3] = '\0';
		g_playerBody[0] = '-';
		g_playerBody[1] = '|';
		g_playerBody[2] = ' ';
		g_playerBody[3] = '\0';
		g_playerLegs[0] = '/';
		g_playerLegs[1] = '|';
		g_playerLegs[2] = ' ';
		g_playerLegs[3] = '\0';
    }

    if (keyPressed[K_RIGHT] && g_worldGrid[charLocation.X + 2][charLocation.Y] == 0 && g_worldGrid[charLocation.X + 2][charLocation.Y - 1] == 0 && g_worldGrid[charLocation.X + 2][charLocation.Y - 2] == 0) {
        charLocation.X++;
		g_playerHead[0] = ' ';
		g_playerHead[1] = '@';
		g_playerHead[2] = ' ';
		g_playerHead[3] = '\0';
		g_playerBody[0] = ' ';
		g_playerBody[1] = '|';
		g_playerBody[2] = '-';
		g_playerBody[3] = '\0';
		g_playerLegs[0] = ' ';
		g_playerLegs[1] = '|';
		g_playerLegs[2] = '\\';
		g_playerLegs[3] = '\0';
    }

	if (keyPressed[K_RIGHT] == false && keyPressed[K_LEFT] == false) {
		g_playerHead[0] = '\\';
		g_playerHead[1] = '@';
		g_playerHead[2] = '/';
		g_playerHead[3] = '\0';
		g_playerBody[0] = ' ';
		g_playerBody[1] = '|';
		g_playerBody[2] = ' ';
		g_playerBody[3] = '\0';
		g_playerLegs[0] = '/';
		g_playerLegs[1] = ' ';
		g_playerLegs[2] = '\\';
		g_playerLegs[3] = '\0';
	}

}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (keyPressed[K_ESCAPE])
        g_quitGame = true;
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    console.clearBuffer(0x1F);
}
void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

}

void renderCharacter()
{
    // Draw the location of the character
    console.writeToBuffer(charLocation, g_playerLegs[1], 0x0F);
	console.writeToBuffer(charLocation.X - 1, charLocation.Y, g_playerLegs[0], 0x0F);
	console.writeToBuffer(charLocation.X + 1, charLocation.Y, g_playerLegs[2], 0x0F);
	console.writeToBuffer(charLocation.X, charLocation.Y - 1, g_playerBody[1], 0x0F);
	console.writeToBuffer(charLocation.X - 1, charLocation.Y - 1, g_playerBody[0], 0x0F);
	console.writeToBuffer(charLocation.X + 1, charLocation.Y - 1, g_playerBody[2], 0x0F);
	console.writeToBuffer(charLocation.X, charLocation.Y - 2, g_playerHead[1], 0x0F);
	console.writeToBuffer(charLocation.X - 1, charLocation.Y - 2, g_playerHead[0], 0x0F);
	console.writeToBuffer(charLocation.X + 1, charLocation.Y - 2, g_playerHead[2], 0x0F);
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / deltaTime << "FPS";
    c.X = console.getConsoleSize().X - 9;
    c.Y = 0;
    console.writeToBuffer(c, ss.str());

}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    console.flushBufferToConsole();
}