/*
Wonder Paint Beta 1.0.00.125
Copyright 2019 Wonder Corp.
Created by Nathan Keith
Programmed by Nathan Keith

*/


#include <SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "mainWindow.h"
using namespace std;


void splashScreen();


int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	splashScreen();
	mainWindow WonderObject;
	WonderObject.init();
	WonderObject.displayLoop();
	WonderObject.save();
	return 0;
}

void splashScreen()
{
	//Size of splash screen window
	int sHeight = 800;
	int sWidth = 1600;

	//Splash window
	SDL_Window* sWindow = NULL;

	//The splash artwork
	SDL_Surface* splash = NULL;

	//Splash stretched to window
	SDL_Surface* optimizedSplash = NULL;

	//Screen surface to blit splash to
	SDL_Surface* sScreenSurface = NULL;
	
	//Initializing window
	sWindow = SDL_CreateWindow("Welcome", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sWidth, sHeight, SDL_WINDOW_SHOWN);
	sScreenSurface = SDL_GetWindowSurface(sWindow);

	splash = SDL_LoadBMP("Wonder_Splash.bmp");
	if (splash != NULL)
	{
		// Stretching the splash to the screen
		optimizedSplash = SDL_ConvertSurface(splash, sScreenSurface->format, NULL);
		SDL_FreeSurface(splash); // Deleting old surface
		SDL_Rect splashRect; // SDL needs these rectangles to stretch to
		splashRect.x = 0;
		splashRect.y = 0;
		splashRect.w = sWidth;
		splashRect.h = sHeight;
		SDL_BlitScaled(optimizedSplash, NULL, sScreenSurface, &splashRect);
		SDL_UpdateWindowSurface(sWindow);
		SDL_Delay(1000); // Just shows the splash for a second
		SDL_FreeSurface(optimizedSplash);
		optimizedSplash = NULL;
		SDL_DestroyWindow(sWindow);
		sWindow = NULL;
	}
	//SDL_BlitScaled(optimizedSurface, NULL, gScreenSurface, &stretchRect)
	else
	{
		printf("Error 125: Splash was not found. Try reinstalling the program.");
	}

}

