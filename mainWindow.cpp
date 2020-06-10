#include "mainWindow.h"




mainWindow::mainWindow()
{
	color = { 0, 0, 0 };
	mWindow = NULL;
	canvas = NULL;
	optimizedCanvas = NULL;
	colorMenu = NULL;
	mScreenSurface = NULL;
	icon = NULL;
	shopOpen = 0;
}



void mainWindow::iconInit()
{
	icon = SDL_LoadBMP("Wonder_Icon.bmp");
	SDL_SetWindowIcon(mWindow, icon);
}
void mainWindow::init()
{
	inputFile();
	mWindow = SDL_CreateWindow("Wonder Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	mScreenSurface = SDL_GetWindowSurface(mWindow);
	colorMenu = SDL_LoadBMP("Wonder_Menu.bmp");
	if (colorMenu == NULL)
	{
		printf("colormenu.bmp is not found. Try reinstalling the program.\n");
	}
	iconInit();

}
void mainWindow::inputFile()
{
	//User input
	char option;
	// A flag for the loop
	bool flag = 0;
	while (!flag)
	{
		printf("Would you like to create a new canvas? (y/n)\n");
		try
		{
			cin >> option;
			if (!cin) // Not a character
			{
				cin.clear(); // Trashing bad input
				cin.ignore(100, '\n');
				throw(invalid_argument("Invalid option. Please try again."));

			}
			else if (option != 'y' && option != 'n' && option != 'N' && option != 'Y') // Bad character
			{
				cin.clear(); // Trashing bad input
				cin.ignore(100, '\n');
				throw(invalid_argument("Invalid option. Please try again."));

			}
			else
			{
				flag = 1;
				cin.clear(); // freeing the input stream;
				cin.ignore(100, '\n');
			}
		}
		catch (invalid_argument ia)
		{
			cout << ia.what() << endl;
		}
	}
	if (option == 'y' || option == 'Y')
	{
		canvas = SDL_CreateRGBSurface(0, 16, 16, 24, 0xFF0000, 0x00FF00, 0x0000FF, 0); // Creates a new canvas surface
		SDL_FillRect(canvas, 0, 0xFFFFFF); // Fills the surface with white pixels

	}
	else
	{
		flag = 0;
		string filePath;
		while (!flag) // loop for getting file path input
		{
			printf("What is the file name of the picture you want to load? \n");
			try
			{
				cin >> filePath;
				if (!cin) // Not a string
				{
					cin.clear(); // Trashing bad input
					cin.ignore(100, '\n');
					throw(invalid_argument("Invalid path. Please try again."));
				}
				const char* convertPath = filePath.c_str(); // I need to convert the path to a character array to use it with LoadBMP
				canvas = SDL_LoadBMP(convertPath);
				if (canvas == NULL)
				{
					cin.clear(); // Trashing bad input
					cin.ignore(100, '\n');
					throw(invalid_argument("Invalid path. Please try again."));
				}
				else
				{
					flag = 1;
				}

				//optimizedCanvas = SDL_ConvertSurface(canvas, mScreenSurface->format, NULL);
			}
			catch (invalid_argument ia)
			{
				cout << ia.what() << endl;
			}

		}
	}


}

void mainWindow::MenuChoice(int x, int y, SDL_Color & color)
{
	if (x > height + 6 && x < height + 131)
	{
		if (y < 125)
		{
			// red
			color = { 0xFF,0,0 };
		}
		else if (y < 250)
		{
			// green
			color = { 0,0xD2,0 };
		}
		else if (y < 375)
		{
			// blue
			color = { 0,0,0xFF };
		}
		else if (y < 500)
		{
			// yellow
			color = { 0xFF,0xFF,0 };
		}
		else if (y < 625)
		{
			// black
			color = { 0,0,0 };
		}
		else if (y < 750)
		{
			// white
			color = { 0xFF,0xFF,0xFF };
		}
		else if (y >= 759)
		{
			// opens paint shop
			paintShop();
		}

	}
}

void mainWindow::putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	// I got this code off of the SDL wiki at sdl.beuc.net/sdl.wiki/Pixel_Access
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	//printf("moss %i \n", *p);
	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *)p = pixel;
		break;

	case 3: // This is they one we're using mostly for wonder paint since of 24 bit color
		p[0] = pixel & 0xff;
		p[1] = (pixel >> 8) & 0xff;
		p[2] = (pixel >> 16) & 0xff;
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
		//default:
			//printf("This picture has %i bypes per pixel. That is too many for wonder paint to handle!\n", bpp);
			//break;
	}
}

Uint32 mainWindow::colorConvert(SDL_Color color)
{
	// Using shift operator to convert to a single integer
	return (color.r << 16) + (color.g << 8) + color.b;
}

void mainWindow::displayLoop()
{
	// Stretching surface to screen
	optimizedCanvas = SDL_ConvertSurface(canvas, mScreenSurface->format, NULL);
	SDL_FreeSurface(canvas);

	// flag for the loop
	bool quit = false;

	// event caused by user input
	SDL_Event e;

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT) // x button
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) // keyboard event
			{
				keyIn(e.key.keysym.sym);

			}
			// Mouse is moving on screen
			else if (e.type == SDL_MOUSEMOTION)
			{
				int x, y; // mouse location
				SDL_GetMouseState(&x, &y); // records mouse position
				//SDL_MouseMotionEvent(&x, &y);
				if (x < height)
				{
					x = x / (height / 16);
					y = y / (height / 16);
					//Left mouse button
					if (SDL_GetMouseState(NULL, NULL)& SDL_BUTTON(1))
					{
						
						
						putpixel(optimizedCanvas, x, y, colorConvert(color));
						
						//printf("Mouse Button 1(left) is pressed.\n");
					}
					// Right mouse button (eraser)
					else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3))
					{
						putpixel(optimizedCanvas, x, y, 0xFFFFFF);
					}
				}
			}
			// Mouse is clicked but not moving
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y; // mouse location
				SDL_GetMouseState(&x, &y); // records mouse position
				if (x < height)
				{
					x = x / (height / 16);
					y = y / (height / 16);
					//Left mouse button
					if (SDL_GetMouseState(NULL, NULL)& SDL_BUTTON(1))
					{
						if (!shopOpen)
						putpixel(optimizedCanvas, x, y, colorConvert(color));
					}
					//Right mouse button (eraser)
					else if (SDL_GetMouseState(NULL, NULL)& SDL_BUTTON(3))
					{
						putpixel(optimizedCanvas, x, y, 0xFFFFFF);
					}
				}
				// You can only choose colors if the mouse isn't moving
				else
				{
					MenuChoice(x, y, color);
				}


			}
		}
		// SDL requires these rectangles to place the pictures on screen
		SDL_Rect stretchRect; // for canvas
		stretchRect.x = 0;
		stretchRect.y = 0;
		stretchRect.w = height;
		stretchRect.h = height;
		SDL_Rect menuRect; // for color menu
		menuRect.x = 800;
		menuRect.y = 0;
		menuRect.w = 200;
		menuRect.h = 800;
		SDL_BlitScaled(optimizedCanvas, NULL, mScreenSurface, &stretchRect);
		SDL_BlitSurface(colorMenu, NULL, mScreenSurface, &menuRect);



		SDL_UpdateWindowSurface(mWindow);
	}
	// Freeing memory used for window
	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
}

void mainWindow::keyIn(SDL_Keycode e)
{
	switch (e)
	{
	case SDLK_r: // red
		//color = 0xFF0000;
		color = { 0xFF,0,0 };
		break;
	case SDLK_g: // green
		//color = 0x00FF00;
		//color = { 0,0xD2,0 };
		color = { 0,0xFF,0 };
		break;
	case SDLK_b: // blue
		//color = 0x0000FF;
		color = { 0,0,0xFF };
		break;
	case SDLK_w: // white
		//color = 0xFFFFFF;
		color = { 0xFF,0xFF,0xFF };
		break;
	case SDLK_y: // yellow
		//color = 0xFFFF00;
		color = { 0xFF,0xFF,0 };
		break;
	case SDLK_k: // black
		//color = 0x000000;
		color = { 0,0,0 };
		break;
	case SDLK_o: // orange
		//color = 0xFFA500;
		color = { 0xFF,0xA5,0 };
		break;
	// up down, equals and minus all create shades and tints
	case SDLK_UP:
		if (color.r < 0xFF)
			color.r = color.r + 1;
		if (color.g < 0xFF)
			color.g = color.g + 1;
		if (color.b < 0xFF)
			color.b = color.b + 1;
		break;
	case SDLK_DOWN:
		if (color.r > 0x00)
			color.r = color.r - 1;
		if (color.g > 0x00)
			color.g = color.g - 1;
		if (color.b > 0x00)
			color.b = color.b - 1;
		break;
	case SDLK_EQUALS:
		if (color.r < 0xFE)
			color.r = color.r + 0x2;
		if (color.g < 0xFE)
			color.g = color.g + 0x2;
		if (color.b < 0xFE)
			color.b = color.b + 0x2;
		break;
	case SDLK_MINUS:
		if (color.r > 0x01)
			color.r = color.r - 0x2;
		if (color.g > 0x01)
			color.g = color.g - 0x2;
		if (color.b > 0x01)
			color.b = color.b - 0x2;
		break;
	}
}

void mainWindow::paintShop()
{
	//Size of paint shop window
	int pHeight = 250;
	int pWidth = 250;

	// Preventing events from happening below the shop window
	shopOpen = 1;

	//Shop window
	SDL_Window* pWindow = NULL;

	//The paint shop photo
	SDL_Surface* shop = NULL;

	//Screen surface to blit shop to
	SDL_Surface* pScreenSurface = NULL;

	//Initializing window
	pWindow = SDL_CreateWindow("Paint Shop", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pWidth, pHeight, SDL_WINDOW_SHOWN);
	pScreenSurface = SDL_GetWindowSurface(pWindow);

	shop = SDL_LoadBMP("Wonder_Shop.bmp");
	if (shop != NULL)
	{
		
		shopOpen = 1;

		// flag for the loop
		bool quitShop = false;

		// event caused by user input
		SDL_Event event;

		while (!quitShop)
		{
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_WINDOWEVENT) // Handles events like resizing or hitting minimize
				{
					if (event.window.event == SDL_WINDOWEVENT_CLOSE) // X button. SDL_QUIT won't work on a secondary window
					{
						quitShop = true;
					}
				}
				// Mouse is clicked but not moving
				else if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					int x, y; // location of mouse
					SDL_GetMouseState(&x, &y); // records mouse position
					if (x < pWidth && y < pHeight)
					{
						//Left mouse button
						if (SDL_BUTTON(1))
						{
							shopColor(x,y);
							quitShop = true;
						}
					
					}

					


				}
			}
			SDL_Rect shopRect;
			shopRect.x = 0;
			shopRect.y = 0;
			shopRect.w = pWidth;
			shopRect.h = pHeight;
			SDL_BlitSurface(shop, NULL, pScreenSurface, &shopRect);
			SDL_UpdateWindowSurface(pWindow);
			
		}
		SDL_FreeSurface(shop);
		shop = NULL;
		SDL_DestroyWindow(pWindow);
		pWindow = NULL;
		SDL_Delay(200); // This delay prevents you from painting on the canvas while the paint shop is open
		shopOpen = 0;
	}
	else
	{
		printf("Error 125: Shop file was not found. Try reinstalling the program.");
	}
}

void mainWindow::shopColor(int x, int y)
{
	int width = 125;
	int height = width;
	if (x < width && y < height)
	{
		// orange
		color = { 0xFF,0xA5,0 };
	}
	else if (x < width)
	{
		// brown
		color = { 0x80, 0x40, 0 };
	}
	else if (y < height)
	{
		// purple
		color = { 0x8C, 0, 0x8C };
	}
	else
	{
		//cyan
		color = { 0, 0xD7, 0xD7 };
	}
}


void mainWindow::save()
{
	//User input
	char option;
	// A flag for the loop
	bool flag = 0;
	while (!flag)
	{
		printf("Would you like to save the file? (y/n)\n");
		try
		{
			cin >> option;
			if (!cin) // Not a character
			{
				cin.clear(); // Trashing bad input
				cin.ignore(100, '\n');
				throw(invalid_argument("Invalid option. Please try again."));

			}
			else if (option != 'y' && option != 'n' && option != 'N' && option != 'Y') // Bad character
			{
				cin.clear(); // Trashing bad input
				cin.ignore(100, '\n');
				throw(invalid_argument("Invalid option. Please try again."));

			}
			else
			{
				flag = 1;
				cin.clear(); // freeing the input stream;
				cin.ignore(100, '\n');
			}
		}
		catch (invalid_argument ia)
		{
			cout << ia.what() << endl;
		}
	}
	if (option == 'y' || option == 'Y')
	{
		flag = 0;
		string filePath;
		while (!flag) // loop for getting file path input
		{
			printf("What is the file name you would like to save the file as?\n");
			try
			{
				cin >> filePath;
				if (!cin) // Not a string
				{
					cin.clear(); // Trashing bad input
					cin.ignore(100, '\n');
					throw(invalid_argument("Invalid path. Please try again."));
				}
				const char* convertPath = filePath.c_str(); // I need to convert the path to a character array to use it with LoadBMP
				
				if (!checkPath(convertPath)) // If it's not a bmp path
				{
					cin.clear(); // Trashing bad input
					cin.ignore(100, '\n');
					throw(invalid_argument("Invalid path. Please try again."));
				}

				if (SDL_SaveBMP(optimizedCanvas, convertPath)<0) // If it can't save then the function will output a negative number
				{
					cin.clear(); // Trashing bad input
					cin.ignore(100, '\n');
					throw(invalid_argument("Invalid path. Please try again."));
				}
				else
				{
					flag = 1;
				}

				//optimizedCanvas = SDL_ConvertSurface(canvas, mScreenSurface->format, NULL);
			}
			catch (invalid_argument ia)
			{
				cout << ia.what() << endl;
			}

		}

	}
}

bool mainWindow::checkPath(const char * path)
{
	// Flag for success
	bool check = 1;

	// The length of the path
	int length = strlen(path);
	//int length = 8;
	if (path[length - 1] != 'p' && path[length - 1] != 'P')
	{
		return 0;
	}
	if (path[length - 2] != 'm' && path[length - 2] != 'M')
	{
		return 0;
	}
	if (path[length - 3] != 'b' && path[length - 3] != 'B')
	{
		return 0;
	}
	if (path[length - 4] != '.')
	{
		return 0;
	}
	return 1;
}


mainWindow::~mainWindow()
{

	// Freeing memory for the pictures loaded
	SDL_FreeSurface(optimizedCanvas);
	optimizedCanvas = NULL;

	SDL_FreeSurface(colorMenu);
	colorMenu = NULL;

	SDL_FreeSurface(icon);
	icon = NULL;


	SDL_Quit();


}
