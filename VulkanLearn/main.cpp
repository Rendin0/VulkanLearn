#include "first_app.hpp"
#include <iostream>
#include <ctime>

int main()
{
	srand(static_cast<int>(time(nullptr)));
	lve::FirstApp app;

	try
	{
		app.run();
	}
	catch (const std::exception &e) 
	{
		std::cout << e.what() << std::endl;
		return 1;
	}


	return 0;
}