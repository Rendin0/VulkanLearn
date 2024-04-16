#include "lve_window.hpp"

namespace lve
{
	LveWindow::LveWindow(int width, int height, std::string window_name): width(width), heigth(height), window_name(window_name)
	{
		initWindow();
	}

	LveWindow::~LveWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool LveWindow::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void LveWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, heigth, window_name.c_str(), nullptr, nullptr);
	}
}