#include "first_app.hpp"
#include <iostream>


int main()
{
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