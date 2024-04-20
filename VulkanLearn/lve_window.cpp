#include "lve_window.hpp"
#include <stdexcept>

namespace lve
{
	LveWindow::LveWindow(int width, int height, std::string window_name) : width(width), height(height), window_name(window_name)
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

	void LveWindow::setKeyCallback(const GLFWkeyfun& keyProcess)
	{
		glfwSetKeyCallback(window, keyProcess);
	}

	void LveWindow::setWindowUserPointer(void* ptr)
	{
		glfwSetWindowUserPointer(window, ptr);
	}

	void LveWindow::createWindowSurface(VkInstance vk_instance, VkSurfaceKHR* vk_surface_khr)
	{
		if (glfwCreateWindowSurface(vk_instance, window, nullptr, vk_surface_khr) != VK_SUCCESS)
			throw std::runtime_error("Surface not success");


	}

	VkExtent2D LveWindow::getExtend()
	{
		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}

	void LveWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
	}
}