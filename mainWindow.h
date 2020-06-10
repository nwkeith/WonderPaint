#pragma once
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

const int width = 1000;
const int height = 800;

class mainWindow
{
private:
	// The current color for painting
	SDL_Color color;

	//The main window that contains the canvas and color menu
	SDL_Window* mWindow;

	// The picture loaded into the program
	SDL_Surface* canvas;

	// The same picture scaled up for the window (which is much bigger than 16x16)
	SDL_Surface* optimizedCanvas;

	//The color menu on the side for picking colors to paint with
	SDL_Surface* colorMenu;

	// The main window surface to blit surfaces to
	SDL_Surface* mScreenSurface;

	//The icon in the top corner of the window
	SDL_Surface * icon;

	// Checks if the shop is open so painting doesn't occur when the shop is open
	bool shopOpen;

public:

	// Constructor for initializing the variables
	mainWindow();

	// Destructor that frees memory
	~mainWindow();

	// Initializes the icon in the window
	void iconInit();

	// Initializes the main window and the canvas
	void init();

	// Asks for the file to import or makes a new canvas
	void inputFile();

	// Changes color when you click on the color menu
	void MenuChoice(int x, int y, SDL_Color & color);

	// Changes color of pixels
	void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

	// converts from a SDL_Color to a Uint32
	Uint32 colorConvert(SDL_Color color);

	// Displays image on screen
	void displayLoop();

	// Handles keyboard input
	void keyIn(SDL_Keycode);

	// Opens the paintshop
	void paintShop();

	// Changes the color to the color you pick in the shop
	void shopColor(int, int);

	// Saves the file
	void save();

	// Checks if the file path given ends in .bmp
	bool checkPath(const char *);
};

