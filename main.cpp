#include "Game.h"

#include <stdexcept>
#include <iostream>


int main()
{
	srand((unsigned int)time(NULL));
	try
	{
		Game app;
		app.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}