#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace lve
{
	class LveWindow
	{
	public:
		LveWindow(int width, int length, std::string window_name);
		~LveWindow();

		LveWindow(const LveWindow&) = delete;
		LveWindow& operator=(const LveWindow&) = delete;

		bool shouldClose();

	private:
		GLFWwindow* window;
		std::string window_name;

		const int width;
		const int heigth;

		void initWindow();

	};
}